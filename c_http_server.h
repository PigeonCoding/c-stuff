#pragma once

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define C_STRING_IMPLEMENTATIONs
#include "c_string.h"

// version 0.1

#define BUFFER_SIZE 1024 * 2

#ifndef THREAD_NUM_SERVER
#define THREAD_NUM_SERVER 8
#endif // THREAD_NUM_SERVER
#ifndef QUEUE_LEN_SERVER
#define QUEUE_LEN_SERVER 256
#endif // QUEUE_LEN_SERVER

#ifdef C_SERVER_MULTI
#include <pthread.h>
typedef struct {
  pthread_t thread;
  int client;
  string res;
  string buff_string;
  char buffer[BUFFER_SIZE];
} Worker;
// pthread_attr_t attr;
Worker worker[THREAD_NUM_SERVER] = {0};
#endif

#define client_accept(s)                                                       \
  accept((s).socket_server, (struct sockaddr *)&client_addr, &addr_len);       \
  if (client_socket < 0) {                                                     \
    perror("Accept failed");                                                   \
    continue;                                                                  \
  };

enum HTTP_CODE {
  c200_text,
  c200_html,
  c404,
  c503,
};

typedef struct {
  int port;
  int socket_server;
  struct sockaddr_in server_addr;
} server;

struct sockaddr_in client_addr;
socklen_t addr_len = sizeof(client_addr);
int client_socket;

void init_threads();

server init_server(int port) {
  server s = {0};
  s.port = port;
  s.socket_server = socket(AF_INET, SOCK_STREAM, 0);
  if (s.socket_server == -1) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }
  s.server_addr.sin_family = AF_INET;
  s.server_addr.sin_addr.s_addr = INADDR_ANY;
  s.server_addr.sin_port = htons(s.port);

  if (bind(s.socket_server, (struct sockaddr *)&s.server_addr,
           sizeof(s.server_addr)) < 0) {
    perror("Bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(s.socket_server, QUEUE_LEN_SERVER) < 0) {
    perror("Listen failed");
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %d\n", s.port);

#ifdef C_SERVER_MULTI
  init_threads();
#endif

  return s;
}

void close_server(server *s) { close(s->socket_server); }

string_view get_path(string full) {

  string_view s = {0};
  size_t sss = 0;
  for (size_t i = 0; i < full.length; i++) {
    if (s.base_pointer == 0 && pseudo_str(full)[i] == ' ') {
      i++;
      s.base_pointer = full.base_pointer + i * char_size;
      sss = i;
    }
    if (s.base_pointer != 0 &&
        strncmp((char *)s.base_pointer + (i - sss) * char_size, " HTTP", 5) ==
            0) {
      break;
    }
    if (s.base_pointer != 0) {
      s.length++;
    }
  }
  return s;
}

string_view get_ip(string full) {
  string_view s = {0};
  size_t sss = 0;
  for (size_t i = 0; i < full.length; i++) {

    // printf("%c", pseudo_str(full)[i]);
    if (s.base_pointer == 0 &&
        strncmp((char *)full.base_pointer + (i)*char_size, "Host:", 5) == 0) {
      i += 6;
      s.base_pointer = (char *)full.base_pointer + (i)*char_size;
      sss = i;
    }
    if (s.base_pointer != 0 &&
        strncmp((char *)s.base_pointer + (i - sss) * char_size, "User-Agent",
                10) == 0) {
      s.length--;
      break;
    }
    if (s.base_pointer != 0) {
      s.length++;
    }
  }

  return s;
}

void gen_res(string *s, int code) {
  switch (code) {
  default:
  case c200_text:
    push_char_ptr(s, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n");
    break;
  case c200_html:
    push_char_ptr(
        s, "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n");
    break;
  case c404:
    push_char_ptr(s, "HTTP/1.1 404 Not Found\r\nContent-Type: "
                     "text/html\r\n\r\n");
    break;
  case c503:
    push_char_ptr(
        s,
        "HTTP/1.1 503 Service Unavailable\r\nServer: Custom\r\nContent-Type: "
        "text/html; charset=UTF-8\r\nRetry-After: 300\r\n");
    break;
  }
}

#ifndef C_SERVER_MULTI

void handle_paths_single(char *buffer, string *buff_string, string *res);

void *income_single(int client_socket_income) {
  if (client_socket_income < 0) {
    return NULL;
  }

  char buffer[BUFFER_SIZE];
  string buff_string = alloc_string();
  string res = alloc_string();
  recv(client_socket_income, buffer, sizeof(buffer), 0);
  push_char_ptr(&buff_string, &(buffer[0]));

  if (get_path(buff_string).length == 0 || get_ip(buff_string).length == 0) {
    V_FPRINTF(stdout, "[WARN]: error with req: " str_fmt "\n",
              print_str(buff_string));
    return NULL;
  }

  handle_paths_single(&buffer[0], &buff_string, &res);

  push_char_to_string(&res, '\n');

  send(client_socket_income, pseudo_str(res), res.length, 0);
  close(client_socket_income);
  free_string(&res);
  free_string(&buff_string);
  return NULL;
}

// void handle_paths_single(char *buffer, string *buff_string, string *res) {
//   gen_res(res, c200_text);
//   push_char_ptr(res, "Hello, World");
// }

// int main(void) {
//   server s = init_server(8080);
//   while (1) {
//     client_socket = client_accept();
//     income_single(client_socket);
//   }
//   close_server(&s);
//   return 0;
// }

#else

void handle_paths_multi(Worker *current);

void *income_multi() {
  int i = -1;
  for (i = 0; i < THREAD_NUM_SERVER; i++) {
    if (worker[i].client == -1) {
      worker[i].client = -2;
      break;
    }
  }

  if (i >= THREAD_NUM_SERVER) {
    V_FPRINTF(stderr, "[ERROR]: no worker?\n");
    return NULL;
  }
  V_FPRINTF(stdout, "[INFO]: thread %d:%ld started\n", i, worker[i].thread);

  while (1) {
    if (worker[i].client >= 0) {
      recv(worker[i].client, worker[i].buffer, sizeof(worker[i].buffer), 0);
      push_char_ptr(&worker[i].buff_string, &(worker[i].buffer[0]));

      string_view path = get_path(worker[i].buff_string);
      string_view ip = get_ip(worker[i].buff_string);

      if (path.length == 0 || ip.length == 0) {
        V_FPRINTF(stdout, "[WARN]: error with req: " str_fmt "\n",
                  print_str(worker[i].buff_string));
        worker[i].client = -2;
      } else {

        handle_paths_multi(&worker[i]);

        push_char_to_string(&worker[i].res, '\n');

        send(worker[i].client, pseudo_str(worker[i].res), worker[i].res.length,
             0);
        close(worker[i].client);

        worker[i].client = -2;
        reset_string(&worker[i].buff_string);
        reset_string(&worker[i].res);
      }
    }
    usleep(20);
  }
  return NULL;
}

void init_threads() {
  for (int i = 0; i < THREAD_NUM_SERVER; i++) {
    worker[i].client = -1;
    worker[i].buff_string = alloc_string();
    worker[i].res = alloc_string();
    pthread_create(&(worker[i].thread), NULL, income_multi, NULL);
    usleep(5);
  }
}

int find_available_worker() {
  int z = -1;
  for (z = 0; z < THREAD_NUM_SERVER; z++) {
    if (worker[z].client == -2) {
      return z;
    }
  }
  return -1;
}

// void handle_paths_multi(Worker *current) {
//   string_view path = get_path(current->buff_string);
//  if (str_cmp(path, "/")) {
//   gen_res(&current->res, c200_text);
//   string_view ip = get_ip(current->buff_string);
//   push_string_view(&current->res, ip);
// } else if (str_cmp(path, "/home")) {
//   gen_res(&current->res, c200_html);
//   string page = alloc_string();
//   read_file(&page, "index.html");
//   push_string_view(&current->res,
//                    (string_view){.base_pointer = page.base_pointer,
//                                  .length = page.length});
//   free_string(&page);
// }else {
//   gen_res(&current->res, c404);
// }
// }

// int main() {
//   server s = init_server(8081);

//   init_threads();

//   while (1) {
//     client_socket = client_accept();
//     int z = find_available_worker();

//     if (z != -1) {
//       worker[z].client = client_socket;
//     } else {
//       fprintf(stdout, "no free worker\n");
//       string s = alloc_string();

//       gen_res(&s, c503);
// push_char_ptr(&current->res,
// "<html><head><title>Error 404</title></head><body><h1>not "
// "found lol</h1></body></html>");
//       send(client_socket, pseudo_str(s), s.length, 0);
//       close(client_socket);
//       free_string(&s);
//     }

//     usleep(20);
//   }

//   close_server(&s);
//   return 0;
// }

#endif
