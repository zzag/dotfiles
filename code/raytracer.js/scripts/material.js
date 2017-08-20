var Vector = require('./vector');

// Air: 1.0
// Water: 1.33
// Ethanol: 1.36
// Olive oil: 1.47
// Ice: 1.31
// Soda glass: 1.46
// Diamond: 2.42


function Material(options) {
  if (!options) {
    options = {};
  }
  this.color = options.color || new Vector(0.6, 0.6, 0.6);
  this.dialectric = options.dialectric || false;
  this.refractionIndex = options.refractionIndex || 1.33;
}


module.exports = Material;
