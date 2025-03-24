#define CLOCKS_PER_US ((unsigned long long) 2112.039831)


/// This overflows if the machine is not restarted in about 50-100 years (on 64 bits archs)
#define RAW_CLOCK_READ() ({ \
			unsigned int lo; \
			unsigned int hi; \
			__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi)); \
			(unsigned long long)(((unsigned long long)hi) << 32 | lo); \
			})
