var Encoder = require('../../lib/encoder'),
  encoder = new Encoder(),
  obj = { foo: 'bar' };

// not a fair test if nothing's listening, since the stream
// hits `highWaterMark` and basically stops processing writes
encoder.on( 'data', function() {} );

module.exports = function() {
  encoder.write( obj );
};
