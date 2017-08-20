function cubic(x, b, c) {
    x = Math.abs(x);
    if (x < 1) {
        return ((12 - 9 * b - 6 * c) * x * x * x
            + (-18 + 12 * b + 6 * c) * x * x 
            + (6 - 2 * b));
    }
    else if (x < 2) {
        return ((-b - 6 * c) * x * x * x
            + (6 * b + 30 * c) * x * x
            + (12 * b - 48 * c) * x
            + (8 * b + 24 * c));
    }
    return 0;
}

function hermite(x) {
    return cubic(x, 0, 0);
}

function bspline(x) {
    return cubic(x, 1, 0);
}

function mitchell(x) {
    return cubic(x, 1.0/3.0, 1.0/3.0);
}

function catmullrom(x) {
    return cubic(x, 0, 0.5);
}

function resample_image(img, width, height, filter) {
    var out = new ImageData(width, height);
    var ratio_w = img.width / width;
    var ratio_h = img.height / height;
    var pos;

    for (var out_i = 0; out_i < height; out_i++) {
        for (var out_j = 0; out_j < width; out_j++) {
            var weight_mass = 0;
            var r = 0;
            var g = 0;
            var b = 0;

            var center_i = out_i * ratio_h;
            var center_j = out_j * ratio_w;
            var in_j_start = Math.floor((out_j - 1) * ratio_w);
            var in_j_end = Math.ceil((out_j + 1) * ratio_w);
            var in_i_start = Math.floor((out_i - 1) * ratio_h);
            var in_i_end = Math.ceil((out_i + 1) * ratio_h);

            in_j_start = Math.max(0, in_j_start);
            in_i_start = Math.max(0, in_i_start);
            in_j_end = Math.min(img.width, in_j_end);
            in_i_end = Math.min(img.height, in_i_end);

            for (var in_i = in_i_start; in_i < in_i_end; in_i++) {
                var dy = center_i - in_i;
                for (var in_j = in_j_start; in_j < in_j_end; in_j++) {
                    var dx = center_j - in_j;
                    var dist = Math.sqrt(dy * dy + dx * dx);
                    var weight = filter(dist);

                    pos = (in_j + in_i * img.width) * 4;
                    r += weight * img.data[pos++];
                    g += weight * img.data[pos++];
                    b += weight * img.data[pos];
                    weight_mass += weight;
                }
            }

            pos = (out_j + out_i * width) * 4;
            out.data[pos++] = r / weight_mass;
            out.data[pos++] = g / weight_mass;
            out.data[pos++] = b / weight_mass;
            out.data[pos] = 255;
        }
    }

    return out;
}
