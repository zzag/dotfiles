using System;
using System.Collections.Generic;
using System.Collections;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace astarconsole
{
    /// <summary>
    /// input data: 2D array
    /// </summary>
    class AStar
    {
        /// <summary>
        /// point
        /// </summary>
        public class Point
        {
            public int m_x;
            public int m_y;

            public Point()
            {
                m_x = m_y = 0;
            }

            public Point(int x, int y)
            {
                m_x = x;
                m_y = y;
            }
        };

        /// <summary>
        /// field
        /// </summary>
        public class Field
        {
            int[,] m_field;

            public Field(int[,] field)
            {
                m_field = field;
            }

            public int width()
            {
                return m_field.GetLength(1);
            }

            public int height()
            {
                return m_field.GetLength(0);
            }

            public int this[int row, int column]
            {
                get
                {
                    if (row < 0 && height() <= row || column < 0 && width() <= column)
                        return -1;

                    return m_field[row, column];
                }

                set
                {
                    if (row < 0 && height() <= row || column < 0 && width() <= column)
                        return;

                    m_field[row, column] = value;
                }
            }
        };

        Field m_field;

        public AStar(AStar.Field field)
        {
            m_field = field;
        }

        /// <summary>
        /// temp class
        /// </summary>
        class Node
        {
            public Point m_position;
            public Node m_parent;
            public int m_G;
            public int m_H;

            public int m_heristicLength
            {
                get
                {
                    return m_G + m_H;
                }
            }
        }

        int heuristicLength(Point start, Point end)
        {
            return Math.Abs(start.m_x - end.m_x) +
                Math.Abs(start.m_y - end.m_y);
        }

        List<Node> getNeighbours(Node parent,
                Point end)
        {
            var retVal = new List<Node>();

            Point[] neighbours = new Point[8];
            neighbours[0] = new Point(parent.m_position.m_x - 1, parent.m_position.m_y - 1);
            neighbours[1] = new Point(parent.m_position.m_x - 1, parent.m_position.m_y);
            neighbours[2] = new Point(parent.m_position.m_x - 1, parent.m_position.m_y + 1);
            neighbours[3] = new Point(parent.m_position.m_x, parent.m_position.m_y - 1);
            neighbours[4] = new Point(parent.m_position.m_x, parent.m_position.m_y + 1);
            neighbours[5] = new Point(parent.m_position.m_x + 1, parent.m_position.m_y - 1);
            neighbours[6] = new Point(parent.m_position.m_x + 1, parent.m_position.m_y);
            neighbours[7] = new Point(parent.m_position.m_x + 1, parent.m_position.m_y + 1);

            foreach(var test in neighbours)
            {
                if (test.m_x < 0 || test.m_x >= m_field.height())
                    continue;

                if (test.m_y < 0 || test.m_y >= m_field.width())
                    continue;

                if (m_field[test.m_x, test.m_y] != 0)
                    continue;

                var neighbourNode = new Node()
                {
                    m_position = test,
                    m_parent = parent,
                    m_G = parent.m_G + 1,
                    m_H = heuristicLength(test, end)
                };

                retVal.Add(neighbourNode);
            }

            return retVal;
        }

        List<Point> getPath(Node path)
        {
            List<Point> retVal = new List<Point>();

            var tmp = path;
            while(tmp.m_parent != null)
            {
                retVal.Add(tmp.m_position);
                tmp = tmp.m_parent;
            }

            retVal.Reverse();
            return retVal;
        }

        public List<Point> getPath(Point start, Point end)
        {
            ///
            /// init open and closed list
            ///
            var openNodes = new List<Node>();
            var closedNodes = new List<Node>();

            ///
            /// init start point
            ///
            Node initNode = new Node()
            {
                m_position = start,
                m_parent = null,
                m_G = 0,
                m_H = heuristicLength(start, end)
            };

            openNodes.Add(initNode);

            ///
            /// main loop
            ///
            while(openNodes.Count > 0)
            {
                // get min
                var currentNode = openNodes.OrderBy(
                    node => node.m_heristicLength).First();

                // is it goal?
                if(currentNode.m_position.m_x == end.m_x &&
                    currentNode.m_position.m_y == end.m_y)
                {
                    return getPath(currentNode);
                }

                // move to closed nodes
                openNodes.Remove(currentNode);
                closedNodes.Add(currentNode);

                foreach(var neighbourNode in getNeighbours(currentNode, end))
                {
                    if (closedNodes.Count(
                        node => node.m_position == neighbourNode.m_position) > 0)
                        continue;

                    var openNode = openNodes.FirstOrDefault(
                        node => node.m_position == neighbourNode.m_position);
                    if (openNode == null)
                        openNodes.Add(neighbourNode);
                    else if(openNode.m_G > neighbourNode.m_G)
                    {
                        openNode.m_parent = currentNode;
                        openNode.m_G = neighbourNode.m_G;
                    }
                }
            }

            return null;
        }
    }
}
