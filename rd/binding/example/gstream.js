var gstream = require('bindings')('gstream');

var obj1 = gstream.createStream(10);
var obj2 = gstream.createStream(20);
var result = gstream.add(obj1, obj2);

console.log(result); // 30
