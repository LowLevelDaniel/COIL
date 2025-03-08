
typedef int coil_err_t;
#define COIL_ERR_GOOD ((coil_err_t)0)

const char *coil_strerr(coil_err_t err);

#ifdef COIL_IMPLMENTATION
  const char *coil_strerr(coil_err_t err) {
    switch (err) {
      case COIL_ERR_GOOD: return "Good";
    };
    return "Not a known error code";
  }