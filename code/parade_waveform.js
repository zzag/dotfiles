function Parade() {
  this.red = null;
  this.green = null;
  this.blue = null;
  this.all = null;
  this.width = 0;
  this.height = 0;
  this.intensity = 0;
}

function build_stats(img) {
  var width = img.width;
  var height = img.height;

  var red = new Uint8ClampedArray(width * 256);
  var green = new Uint8ClampedArray(width * 256);
  var blue = new Uint8ClampedArray(width * 256);

  for (var i = 0; i < height; i++) {
    for (var j = 0; j < width; j++) {
      var pos = 4 * (j + i * width);

      var r = img.data[pos++];
      var g = img.data[pos++];
      var b = img.data[pos++];

      red[j + r * width]++;
      green[j + g * width]++;
      blue[j + b * width]++;
    }
  }

  return {
    red: red,
    green: green,
    blue: blue
  };
}

Parade.prototype.process = function (img, intensity) {
  var width = img.width;
  var height = img.height;
  var pos;

  var stats = build_stats(img);

  this.red = new ImageData(width, 256);
  this.green = new ImageData(width, 256);
  this.blue = new ImageData(width, 256);
  this.all = new ImageData(width, 256);

  var red_data = new Uint32Array(this.red.data.buffer);
  var green_data = new Uint32Array(this.green.data.buffer);
  var blue_data = new Uint32Array(this.blue.data.buffer);
  var all_data = new Uint32Array(this.all.data.buffer);

  for (i = 0; i < 256; i++) {
    for (j = 0; j < width; j++) {
      pos = j + i * width;

      var r = Math.min(255, stats.red[pos] * intensity);
      var g = Math.min(255, stats.green[pos] * intensity) << 8;
      var b = Math.min(255, stats.blue[pos] * intensity) << 16;

      pos = j + (256 - i - 1) * width;
      red_data[pos] = r | 0xff000000;
      green_data[pos] = g | 0xff000000;
      blue_data[pos] = b | 0xff000000;
      all_data[pos] = r | g | b | 0xff000000;
    }
  }

  this.width = width;
  this.height = height;
  this.intensity = intensity;
};
