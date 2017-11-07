# faye browser client


```js
var publication = client.publish('/foo', { text: "Hello" });

publication.then(function () {
  console.log("success");
}, function (error) {
  console.log("error: " + error.message);
});
```

```js
client.on('transport:down', function () {
  // offline
});

client.on('transport:up', function () {
  // online
});
```

```js
client.addExtension({
  incoming: function (message, callback) {
    console.log("received: " + message);
    callback(message);
  },
  outgoing: function (message, callback) {
    console.log("send: " + message);
    callback(message);
  }
});
```
