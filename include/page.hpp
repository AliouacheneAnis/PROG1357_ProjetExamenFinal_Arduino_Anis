const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
    <title>Saise Informations Wifi-Serveur</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <!-- Add Bootstrap CSS link -->
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css">
</head>

<body>
    <div class="container mt-5">
        <h3 class="text-center">Saise Informations Wifi-Serveur</h3>
        <br><br>
        <form action="/get">
            <div class="form-group">
                <label for="ssid">SSID:</label>
                <input type="text" class="form-control" id="ssid" name="ssid">
            </div>
            <div class="form-group">
                <label for="password">Password:</label>
                <input type="password" class="form-control" id="password" name="password">
            </div>
            <div class="form-group">
                <label for="server_ip">Server IP Address:</label>
                <input type="text" class="form-control" id="server_ip" name="server_ip">
            </div>
            <div class="form-group">
                <label for="port">Port:</label>
                <input type="number" class="form-control" id="port" name="port">
            </div>
            <button type="submit" class="btn btn-primary">Submit</button>
        </form>
    </div>

    <!-- Add Bootstrap JS scripts -->
    <script src="https://code.jquery.com/jquery-3.5.1.slim.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.0.7/dist/umd/popper.min.js"></script>
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>

</html>)rawliteral" ;
