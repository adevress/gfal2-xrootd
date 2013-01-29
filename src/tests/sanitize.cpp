#include <iostream>
#include "../gfal_xrootd_plugin_utils.h"



int assert_equal(std::string a, std::string b) {
  if (a != b) {
    std::cout << a << " != " << b << std::endl;
    return 1;
  }
  else {
    std::cout <<  a << " == " << b << std::endl;
    return 0;
  }
}



int main(int argc, char *argv[]) {
  int nerr = 0;

  nerr += assert_equal("root://host.domain///", sanitize_url("root://host.domain"));
  nerr += assert_equal("root://host.domain///", sanitize_url("root://host.domain/"));
  nerr += assert_equal("root://host.domain///", sanitize_url("root://host.domain//"));
  nerr += assert_equal("root://host.domain///path/file", sanitize_url("root://host.domain/path/file"));
  nerr += assert_equal("root://host.domain///path/file", sanitize_url("root://host.domain//path/file"));
  nerr += assert_equal("root://host.domain///path/file", sanitize_url("root://host.domain///path/file"));

  std::cout << nerr << " failures" << std::endl;

  return nerr;
}
