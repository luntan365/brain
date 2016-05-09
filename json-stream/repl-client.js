var json = require('./index');
var net = require('net');

var socket = net.createConnection({ port: 1337 });

var encode = json.encode();
var parse = json.parse();

socket.pipe( parse );

parse.on( 'message', function( msg ) {
  console.log( '\n' + JSON.stringify( msg, null, 2 ) + '\n' );
});

process.stdin.on( 'data', function( text ) {
  var obj, msg;

  try {
    obj = JSON.parse( text );
    encode.write( obj );
    msg = encode.read();
    socket.write( msg );
  } catch ( e ) {}

});
