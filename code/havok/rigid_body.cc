#include <iostream>
#include <Windows.h>

// base Havok headers
#include <Common/Base/hkBase.h>
#include <Common/Base/keycode.cxx>
#include <Common/Base/Config/hkProductFeatures.h>
#include <Common/Base/Config/hkProductFeatures.cxx>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/System/Error/hkDefaultError.h>
#include <Common/Base/Memory/System/hkMemorySystem.h>
#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>
#include <Common/Base/Memory/Allocator/Malloc/hkMallocAllocator.h>
#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Physics2012/Dynamics/World/hkpWorld.h>
#include <Physics2012/Collide/Dispatch/hkpAgentRegisterUtil.h>

// hkpBoxShape
#include <Physics2012/Collide/Shape/Convex/Box/hkpBoxShape.h>

// hkpRigidBodyCinfo
#include <Physics2012/Dynamics/Entity/hkpRigidBody.h>

// hkMassProperties, hkpInertiaTensorComputer
#include <Physics2012/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>

// Havok Visual Debugger headers
#include <Common/Visualize/hkVisualDebugger.h>
#include <Physics2012/Utilities/VisualDebugger/hkpPhysicsContext.h>

#pragma comment(lib, "hkaAnimation.lib")
#pragma comment(lib, "hkaInternal.lib")
#pragma comment(lib, "hkaPhysics2012Bridge.lib")
#pragma comment(lib, "hkBase.lib")
#pragma comment(lib, "hkcdCollide.lib")
#pragma comment(lib, "hkcdInternal.lib")
#pragma comment(lib, "hkCompat.lib")
#pragma comment(lib, "hkgBridge.lib")
#pragma comment(lib, "hkgCommon.lib")
#pragma comment(lib, "hkgDx11.lib")
#pragma comment(lib, "hkGeometryUtilities.lib ")
#pragma comment(lib, "hkgOglES.lib")
#pragma comment(lib, "hkgOglES2.lib")
#pragma comment(lib, "hkgSoundCommon.lib")
#pragma comment(lib, "hkgSoundXAudio2.lib")
#pragma comment(lib, "hkInternal.lib")
#pragma comment(lib, "hkpCollide.lib")
#pragma comment(lib, "hkpConstraint.lib")
#pragma comment(lib, "hkpConstraintSolver.lib")
#pragma comment(lib, "hkpDynamics.lib")
#pragma comment(lib, "hkpInternal.lib")
#pragma comment(lib, "hkpUtilities.lib")
#pragma comment(lib, "hkpVehicle.lib")
#pragma comment(lib, "hkSceneData.lib")
#pragma comment(lib, "hkSerialize.lib")
#pragma comment(lib, "hkVisualize.lib")

using std::cout;
using std::endl;

////
//// Havok code
////
void InitHavok();
void ShutdownHavok();

void InitMemory();
void InitWorld();
void InitVDB();

void AddRigidBodies();

void StepHavok();

////
//// Data
////
bool g_bUseVDB;
hkCpuJobThreadPool *g_pThreadPool;
hkJobQueue *g_pJobQueue;
hkpWorld *g_pWorld;
hkVisualDebugger *g_pVdb;
hkpPhysicsContext *g_pContext;
hkReal g_rStepTime;
hkpRigidBody *box;

int main() {

	g_bUseVDB = true;

	g_rStepTime = 1.0f / 60.0f;

	InitHavok();

	while(true) {

		StepHavok();

		Sleep(16);

	}

	ShutdownHavok();

	return 0;
}

void StepHavok() {

	g_pWorld->stepMultithreaded(g_pJobQueue, g_pThreadPool, g_rStepTime);

	////
	//// Some useful code here
	////

	////
	//// Visual debugger
	////
	if(g_bUseVDB) {
		g_pContext->syncTimers(g_pThreadPool);
		g_pVdb->step(g_rStepTime);
	}

	hkMonitorStream::getInstance().reset();
	g_pThreadPool->clearTimerData();

}

void InitHavok() {

	InitMemory();
	InitWorld();

	if(g_bUseVDB) {
		InitVDB();
	}

	g_pWorld->lock();

	AddRigidBodies();

	g_pWorld->unlock();

}

void AddRigidBodies() {

	////
	//// Add static plane
	////
	{
		hkVector4 halfExtents(20.0f, 2.0f, 20.0f);
		hkpBoxShape *boxShape = new hkpBoxShape(halfExtents);

		hkpRigidBodyCinfo ci;
		ci.m_shape = boxShape;
		ci.m_position = hkVector4(0.0f, -2.0f, 0.0f);
		ci.m_motionType = hkpMotion::MOTION_FIXED;
		boxShape->setRadius(0.001f);

		hkpRigidBody *rigidBody = new hkpRigidBody(ci);
		hkpEntity *createdEntity = g_pWorld->addEntity(rigidBody);

		boxShape->removeReference();
		createdEntity->removeReference();
	}

	////
	//// Add falling box
	////
	{
		hkVector4 halfExtents(0.5f, 0.5f, 0.5f);
		hkpBoxShape *boxShape = new hkpBoxShape(halfExtents);

		hkpRigidBodyCinfo ci;
		ci.m_shape = boxShape;
		ci.m_position = hkVector4(0.0f, 100.0f, 0.0f);
		ci.m_motionType = hkpMotion::MOTION_DYNAMIC;
		boxShape->setRadius(0.001f);

		const hkReal boxMass(1.0f);
		hkMassProperties massProp;
		hkpInertiaTensorComputer::computeShapeVolumeMassProperties(boxShape, boxMass, massProp);

		ci.setMassProperties(massProp);

		hkpRigidBody *rigidBody = new hkpRigidBody(ci);

		box = reinterpret_cast<hkpRigidBody *>(g_pWorld->addEntity(rigidBody));

		boxShape->removeReference();
		rigidBody->removeReference();
	}

}

void ShutdownHavok() {

	g_pWorld->markForWrite();

	g_pWorld->removeReference();

	delete g_pJobQueue;

	g_pThreadPool->removeReference();

	if(g_bUseVDB) {
		g_pVdb->removeReference();
		g_pContext->removeReference();
	}

	hkBaseSystem::quit();
	hkMemoryInitUtil::quit();

}

void onError(const char *msg, void *unused) {

	cout << msg << endl;

}

void InitMemory() {

#if defined(HK_COMPILER_HAS_INTRINSICS_IA32) && HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED

	_MM_SET_DENORMALS_ZERO_MODE(_MM_FLUSH_ZERO_ON);

#endif

	///
	/// Init memory
	/// and base system
	///
	hkMemoryRouter *memoryRouter = hkMemoryInitUtil::initDefault(
		hkMallocAllocator::m_defaultMallocAllocator,
		hkMemorySystem::FrameInfo(1024 * 1024));

	hkBaseSystem::init(memoryRouter, onError);

	///
	/// init threading
	///
	int totalNumThreadsUsed;
	hkHardwareInfo hwInfo;

	hkGetHardwareInfo(hwInfo);

	totalNumThreadsUsed = hwInfo.m_numThreads;

	hkCpuJobThreadPoolCinfo threadPoolInfo;
	threadPoolInfo.m_numThreads = totalNumThreadsUsed - 1;
	threadPoolInfo.m_timerBufferPerThreadAllocation = 200000;

	g_pThreadPool = new hkCpuJobThreadPool(threadPoolInfo);

	hkJobQueueCinfo jobQueueInfo;
	jobQueueInfo.m_jobQueueHwSetup.m_numCpuThreads = totalNumThreadsUsed;
	g_pJobQueue = new hkJobQueue(jobQueueInfo);

	hkMonitorStream::getInstance().resize(200000);

}

void InitWorld() {

	hkpWorldCinfo worldInfo;
	worldInfo.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_MULTITHREADED;
	worldInfo.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_REMOVE_ENTITY;

	g_pWorld = new hkpWorld(worldInfo);
	g_pWorld->m_wantDeactivation = false;

	g_pWorld->markForWrite();

	hkpAgentRegisterUtil::registerAllAgents(g_pWorld->getCollisionDispatcher());
	g_pWorld->registerWithJobQueue(g_pJobQueue);

}

void InitVDB() {

	hkArray<hkProcessContext *> contexts;

	{
		g_pContext = new hkpPhysicsContext();
		hkpPhysicsContext::registerAllPhysicsProcesses();
		g_pContext->addWorld(g_pWorld);
		contexts.pushBack(g_pContext);
		g_pWorld->unmarkForWrite();
	}

	g_pVdb = new hkVisualDebugger(contexts);
	g_pVdb->serve();

}