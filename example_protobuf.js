/**
 * Created by janos4276 on 07/07/17.
 */
'use strict';

const dgram = require('dgram');
const server = dgram.createSocket('udp4');
const pb = require('./proto/example.js');

server.on('error', (err) => {
   console.log('server error:\n${err.stack}');
   server.close();
});

const Message = pb.example.proto.Event;
const client = dgram.createSocket('udp4');

server.on('message', (msg, rinfo) => {
    console.log('server got: %s from %s:%s\n', msg, rinfo.address, rinfo.port);
    let an_event = Message.decode(msg);
    console.log('Decoded message: %j', an_event);

    an_event.language.push({language: 'JavaScript'});
    let buffer = Message.encode(an_event).finish();

    //client.open();
    //client.
    server.send(buffer, 40010, 'localhost');
});

server.on('listening', () => {
    const address = server.address();
    console.log('server listening %s:%s', address.address, address.port);
});

server.bind(40000);
