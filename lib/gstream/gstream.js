var gstream = require('bindings')('gstream');

var obj1 = gstream.createObject(10);
var obj2 = gstream.createObject(20);
var result = gstream.add(obj1, obj2);

console.log(result); // 30
