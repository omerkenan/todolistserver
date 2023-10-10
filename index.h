
const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta charset='utf-8'>
  <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
  <title>To Do List</title>
  <link rel='icon' type='image/png' href='https://emojiisland.com/cdn/shop/products/Emoji_Icon_-_Smiling_grande.png'/>
  <style>
    html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
    body { margin-top: 50px; background-color:#B4C292;}
    h1 { color: #4C061D; margin: 50px auto 30px; }
    p { font-size: 16px; color: #D17A22; margin-bottom: 10px; }
    
  </style>
</head>
<body>
  <h1>To Do List</h1>
  <ul id="eksiklerListesi">
    <!-- TODO'S will be shown here -->
  </ul>
  <input type="text" id="yeniEksik" placeholder="New To Do">
  <input type="button" value="Add" onclick="addEksik()">
  <script>
    var socket = new WebSocket('ws://LOCAL_IP:81/'); // ADD YOUR LOCAL IP HERE!!!

    // Take To Do List from server when page loaded
    window.onload = function () {
      getEksiklerListesi();
    };

    socket.onmessage = function (event) {
  var ul = document.getElementById('eksiklerListesi');
  ul.innerHTML = event.data;
  
  // Add X for each item to remove
  var eksikler = ul.getElementsByTagName('li');
  for (var i = 0; i < eksikler.length; i++) {
    var xButton = document.createElement('span');
    xButton.innerHTML = " X";
    xButton.style.color = "red";
    xButton.style.cursor = "pointer";
    
    // Call removeItem when X clicked and sent message to server
    xButton.onclick = function() {
      removeItem(this.parentElement);
    };
    
    eksikler[i].appendChild(xButton);
  }
};


    function addEksik() {
      var yeniEksikInput = document.getElementById('yeniEksik');
      var yeniEksik = yeniEksikInput.value;
      if (yeniEksik.trim() !== '') {
        socket.send(yeniEksik);
        yeniEksikInput.value = ''; // Metin girişini temizle
      }
    }

    function removeItem(item) {
      // Remove only clicked item
      item.remove();
      
      // Sent message when X clicked
      var message = item.textContent.trim(); // Take text of item, do not include X
      let finalmessage = message.slice(0, -2);
      socket.send("remove:" + finalmessage);
    }


    // When Enter clicked add item as well
    var yeniEksikInput = document.getElementById('yeniEksik');
    yeniEksikInput.addEventListener('keyup', function(event) {
      if (event.key === 'Enter') {
        addEksik();
      }
    });

    function getEksiklerListesi() {
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function () {
    if (xhr.readyState == 4 && xhr.status == 200) {
      var eksiklerListesi = xhr.responseText;
      var ul = document.getElementById('eksiklerListesi');
      ul.innerHTML = eksiklerListesi;
      
      // // Add X for each item to remove
      var eksikler = ul.getElementsByTagName('li');
      for (var i = 0; i < eksikler.length; i++) {
        var xButton = document.createElement('span');
        xButton.innerHTML = " X";
        xButton.style.color = "red";
        xButton.style.cursor = "pointer";
        
        // Call removeItem when X clicked and sent message to server
        xButton.onclick = function() {
          removeItem(this.parentElement);
        };
        
        eksikler[i].appendChild(xButton);
      }
    }
  };
  xhr.open("GET", "/getEksikler", true);
  xhr.send();
}

  </script>
</body>
</html>

)=====";