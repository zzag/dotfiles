function interpolateBezier(p0, p1, p2, t) {
    return (Math.pow(1 - t, 2) * p0
            + 2 * t * (1 - t) * p1
            + t * t * p2);
}

export default (p0, p1, p2, t) => {
    const result = {
        x: interpolateBezier(p0.x, p1.x, p2.x, t),
        y: interpolateBezier(p0.y, p1.y, p2.y, t)
    };
    return result;
};