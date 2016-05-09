var Parser = require('../../lib/parser'),
  Encoder = require('../../lib/encoder'),
  encoder = new Encoder(),
  parser = new Parser(),
  buffer;

encoder.on( 'data', function( chunk ) {
  buffer = chunk;
});

encoder.write({ foo: 'bar' });

module.exports = function() {
  parser.write( buffer );
};
