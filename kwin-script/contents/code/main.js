let lastWrapTime = 0;

function getOuterScreens() {
    const screens = workspace.screens;
    if (!screens || screens.length < 2) return null;

    let leftScreen = screens[0];
    let rightScreen = screens[0];

    for (let i = 1; i < screens.length; i++) {
        const s = screens[i];
        if (s.geometry.x < leftScreen.geometry.x) leftScreen = s;
        if (s.geometry.x + s.geometry.width > rightScreen.geometry.x + rightScreen.geometry.width) rightScreen = s;
    }
    return { left: leftScreen, right: rightScreen };
}

function mapY(y, srcGeo, dstGeo) {
    let ratio = (y - srcGeo.y) / srcGeo.height;
    ratio = Math.max(0.0, Math.min(1.0, ratio));
    const mapped = Math.round(dstGeo.y + ratio * dstGeo.height);
    return Math.max(dstGeo.y, Math.min(dstGeo.y + dstGeo.height - 1, mapped));
}

workspace.cursorPosChanged.connect(function () {
    const now = Date.now();
    if (now - lastWrapTime < 150) return;

    const bounds = getOuterScreens();
    if (!bounds || bounds.left === bounds.right) return;

    const pos = workspace.cursorPos;
    const leftGeo = bounds.left.geometry;
    const rightGeo = bounds.right.geometry;

    const minX = leftGeo.x;
    const maxX = rightGeo.x + rightGeo.width;

    if (pos.x >= maxX - 2) {
        lastWrapTime = now;
        const targetX = minX + 8;
        const targetY = mapY(pos.y, rightGeo, leftGeo);
        callDBus("org.berrybyte.CursorWarper", "/Warper", "org.berrybyte.CursorWarper", "Warp",
                 Math.round(targetX - pos.x), Math.round(targetY - pos.y));
    } else if (pos.x <= minX + 1) {
        lastWrapTime = now;
        const targetX = maxX - 8;
        const targetY = mapY(pos.y, leftGeo, rightGeo);
        callDBus("org.berrybyte.CursorWarper", "/Warper", "org.berrybyte.CursorWarper", "Warp",
                 Math.round(targetX - pos.x), Math.round(targetY - pos.y));
    }
});
