#ifndef DTS_H_
#define DTS_H_

#include <stdint.h>

typedef struct dts_results {
  uint32_t num_dts;
  uint64_t *bytes_stored;
} dts_results;

void
dts_init (uint32_t num_dts);
void
dts_store (uint32_t dtsind, uint64_t bytes);
void
dts_clean ();
dts_results *
dts_get_results();

#endif
