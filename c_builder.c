#include "c_builder.h"
#include "c_nice.h"

#define compiler "cc"
#define builder_name "c_builder"

const char *files_check[] = {"main.c", "c_arena.h", "c_nice.h", "c_string.h",
                             "c_vector.h"};
const char *builder_files[] = {"c_builder.c", "c_builder.h"};

int check_build() {
  if (check_file(builder_files[0]) | check_file(builder_files[1])) {
    push_str_whitespace(cmd, compiler " %s -o " builder_name, builder_files[0]);
    exec_and_reset_and_log(cmd);

    reset_string(&temp);
    push_str_whitespace(temp, "./" builder_name);
    if (first_arg != NULL)
      push_str_whitespace(temp, "%s", first_arg);

    system(get_string_c(&temp));

    return 1;
  }

  for (int i = 0; (size_t)i < array_length(files_check); i++) {
    if (check_file(files_check[i])) {
      push_str_whitespace(cmd, compiler " main.c");
      push_str_whitespace(cmd, "-g -Wall -Wextra -Wswitch-enum");
      push_str_whitespace(cmd, "-o " dir_build "/main");

      exec_and_reset_and_log(cmd);
      break;
    }
  }

  return 0;
}

int main(int argc, char *argv[]) {
  // check_arg_num(2);
  init_builder();

  if (check_arg_is("build")) {
    if (check_build())
      return 0;
  } else {
    if (check_build())
      return 0;
    push_str_whitespace(cmd, "./build/main");
    exec_and_reset(cmd);
  }

  free_string(&cmd);
  free_string(&temp);

  return 0;
}
