#include <cpwn.h>

int
main(int argc, char *const *argv) {
    char *msg = cstr.from_cstr("Hello back");
    serv_sock_t server = tcp.server("127.0.0.1", 1337);

    sock_t client = tcp.next_client(&server, 5);

    printf("%lld\n", tcp.send(&client, msg, cstr.strlen(msg)));
    tcp.shutdown(&client);
    tcp.close(&client);
    tcp.close(&server);
    cstr.dealloc(&msg);
    return 0;
}