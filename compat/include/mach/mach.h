#ifndef CCTOOLS_COMPAT_MACH_H
#define CCTOOLS_COMPAT_MACH_H
#include <mach/boolean.h>
#include <mach/machine.h>
#include <mach/message.h>
#include <mach/vm_prot.h>
typedef int kern_return_t;
typedef int mach_error_t;
typedef unsigned int mach_port_t;
typedef mach_port_t host_t;
typedef int host_flavor_t;
typedef integer_t *host_info_t;
typedef mach_port_t ipc_space_t;
typedef mach_port_t mach_port_name_t;
typedef mach_port_t vm_map_t;
typedef int vm_machine_attribute_val_t;
#define KERN_SUCCESS 0
#define HOST_BASIC_INFO 1
#define HOST_SCHED_INFO 3
#define HOST_PREFERRED_USER_ARCH 12
#define MATTR_CACHE 1
#define MATTR_VAL_ICACHE_FLUSH 2
struct host_basic_info {
  integer_t max_cpus, avail_cpus;
  natural_t memory_size;
  cpu_type_t cpu_type;
  cpu_subtype_t cpu_subtype;
  integer_t cpu_threadtype, physical_cpu, physical_cpu_max;
  integer_t logical_cpu, logical_cpu_max;
  uint64_t max_mem;
};
typedef struct host_basic_info *host_basic_info_t;
#define HOST_BASIC_INFO_COUNT                                                  \
  ((mach_msg_type_number_t)(sizeof(struct host_basic_info) / sizeof(integer_t)))
struct host_sched_info {
  integer_t min_timeout, min_quantum;
};
#define HOST_SCHED_INFO_COUNT                                                  \
  ((mach_msg_type_number_t)(sizeof(struct host_sched_info) / sizeof(integer_t)))
struct host_preferred_user_arch {
  cpu_type_t cpu_type;
  cpu_subtype_t cpu_subtype;
};
#define HOST_PREFERRED_USER_ARCH_COUNT                                         \
  ((mach_msg_type_number_t)(sizeof(struct host_preferred_user_arch) /          \
                            sizeof(integer_t)))
extern mach_port_t mach_task_self_;
extern vm_size_t vm_page_size;
#define mach_task_self() mach_task_self_
mach_port_t mach_host_self(void);
kern_return_t host_info(host_t, host_flavor_t, host_info_t,
                        mach_msg_type_number_t *);
kern_return_t mach_port_deallocate(ipc_space_t, mach_port_name_t);
kern_return_t vm_allocate(vm_map_t, vm_address_t *, vm_size_t, int);
kern_return_t vm_deallocate(vm_map_t, vm_address_t, vm_size_t);
kern_return_t vm_machine_attribute(vm_map_t, vm_address_t, vm_size_t, int,
                                   vm_machine_attribute_val_t *);
char *mach_error_string(mach_error_t);
#endif
