#include "cache.h"

int main() {
  cash::Cache c = cash::Cache<int, int>(1);
  c.put(1, 1);
  c.put(2, 10);
  c.put(3, 100);
  c.put(4, 1000);
}