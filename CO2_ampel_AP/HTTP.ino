/* Function for HTTP Request / */
void handle_OnConnect() {
  server.send(200, "text/html", SendHTML(coWert, temp)); 
}

/* Function for HTTP Error */
void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}
