# json-stream

![Codeship badge](https://codeship.com/projects/dd3a47e0-983a-0133-a753-66b8883e9f8d/status?branch=develop)

Fast, lightweight library for encoding and decoding JSON messages over streams.

This is mainly intended for TCP applications, where the concept of messages
doesn't really exist.

It's entirely possible that a single JSON string could show up on the other
end in three pieces. Or that multiple messages will be combined into a single
buffer.

So, with that in mind, here's my basic solution:

### Streams

If you haven't played with streams, I don't really know where to begin on
that front. They're something you should Google.

If you *have* used streams a bit, you should know that both the parser and
encoder are subclasses of `stream.Transform`, which means they're subclasses
of `stream.Duplex`, which means they implement `readable` and `writable`
stream interfaces.

### Encoding

The encoding part is pretty simple.

1. Take a JavaScript object and convert it to a JSON string
2. Get the `byteLength` of the new JSON string
3. Create a new `Buffer` instance where `length = byteLength + 8`
4. Write the string `'JSON'` as binary data in the first 4 bytes of the buffer
5. Write `byteLength` in bytes 5-8 of the buffer
6. Write the JSON string at byte 9+

### Parsing

This part ends up being a bit tricker, but essentially, I know what a message
should start with and I know how long each one is supposed to be.

So from there, it's really just a matter of counting and state management.

### Examples

##### Piping data

Both streams pipe `Buffer` instances on the way out (like pretty much
all streams), but `encode` expects on object to be passed to `write`.

```js
var json = require('json-stream'),
  // create an encoder stream
  encode = json.encode(),
  // create a parser stream
  parse = json.parse();

encode.pipe( parse ).pipe( process.stdout );

encode.write({ hello: 'world' }); // should log {"hello": "world"}
```

##### `data` events

Both streams implement standard `data` events, which emit `Buffer` instances.

```js
var json = require('json-stream'),
  encode = json.encode(),
  parse = json.parse();

parse.on( 'data', function( buffer ) {
  console.log( buffer instanceof Buffer ); // true
});

encode.write({ foo: 'bar' });
```

##### `message` event

The `parse` stream also implements a custom `message` event for convenience.
Rather than emitting a `Buffer` instance, the `message` event emits a parsed
JavaScript object.

```js
var json = require('json-stream'),
  encode = json.encode(),
  parse = json.parse();

parse.on( 'message', function( obj ) {
  console.log( obj.foo ); // 'bar'
});

encode.write({ foo: 'bar' });
```

##### Writing to sockets

```js
var net = require('net'),
  json = require('json-stream'),
  server = net.createServer();

server.listen( 1337 );

server.on( 'connection', function( socket ) {
  var encode = json.encode();
  encode.pipe( socket );
  encode.write({ hello: 'world' });
});
```

##### Reading from sockets

```js
var net = require('net'),
  json = require('json-stream'),
  client = net.createConnection({ port: 1337 });

client.pipe( json.parse() ).on( 'message', function( obj ) {
  console.log( obj ); // { hello: 'world' }
});
```
