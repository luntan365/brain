var json = require('./index');
var net = require('net');

var server = net.createServer();
var encode = json.encode();

var sockets = [];

server.listen( 1337 );

server.on( 'connection', function( socket ) {
  var parse = json.parse();

  sockets.push( socket );

  socket.pipe( parse );

  parse.on( 'message', function( msg ) {
    console.log( '\n' + JSON.stringify( msg, null, 2 ) + '\n' );
  });

  socket.on( 'close', function() {
    var index = sockets.indexOf( socket );

    if ( index !== -1 ) {
      sockets.splice( index, 1 );
    }

  });

});

process.stdin.on( 'data', function( text ) {
  var obj, msg;

  try {
    obj = JSON.parse( text );
    encode.write( obj );
    msg = encode.read();
    sockets.forEach(function( socket ) {
      socket.write( msg );
    })
  } catch ( e ) {}

});
