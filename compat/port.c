#include <mach/mach.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

mach_port_t mach_task_self_ = 0;
vm_size_t vm_page_size = 4096;

int _NSGetExecutablePath(char *path, unsigned int *size) {
#if defined(__linux__)
  ssize_t length = readlink("/proc/self/exe", path, *size > 0 ? *size - 1 : 0);
  if (length < 0)
    return -1;
  if ((unsigned int)length >= *size) {
    *size = (unsigned int)length + 1;
    return -1;
  }
  path[length] = '\0';
  *size = (unsigned int)length;
  return 0;
#else
  (void)path;
  (void)size;
  return -1;
#endif
}

char *mach_error_string(mach_error_t error) {
  (void)error;
  return "unknown Mach error";
}

mach_port_t mach_host_self(void) { return 0; }

kern_return_t host_info(host_t host, host_flavor_t flavor, host_info_t output,
                        mach_msg_type_number_t *count) {
  (void)host;
  (void)count;
  if (flavor == HOST_BASIC_INFO) {
    struct host_basic_info *info = (struct host_basic_info *)output;
    memset(info, 0, sizeof(*info));
#if defined(__aarch64__) || defined(__arm64__)
    info->cpu_type = CPU_TYPE_ARM64;
    info->cpu_subtype = CPU_SUBTYPE_ARM64_ALL;
#elif defined(__x86_64__)
    info->cpu_type = CPU_TYPE_X86_64;
    info->cpu_subtype = CPU_SUBTYPE_X86_64_ALL;
#elif defined(__i386__)
    info->cpu_type = CPU_TYPE_I386;
    info->cpu_subtype = CPU_SUBTYPE_I386_ALL;
#else
    info->cpu_type = CPU_TYPE_ANY;
#endif
  }
  return KERN_SUCCESS;
}

kern_return_t mach_port_deallocate(ipc_space_t task, mach_port_name_t name) {
  (void)task;
  (void)name;
  return KERN_SUCCESS;
}

kern_return_t vm_allocate(vm_map_t task, vm_address_t *address, vm_size_t size,
                          int anywhere) {
  (void)task;
  (void)anywhere;
  void *allocation = calloc(1, size);
  if (allocation == NULL)
    return ENOMEM;
  *address = (vm_address_t)allocation;
  return KERN_SUCCESS;
}

kern_return_t vm_deallocate(vm_map_t task, vm_address_t address,
                            vm_size_t size) {
  (void)task;
  (void)address;
  (void)size;
  /* Input files are normally mmap'ed; keep the historical port behavior. */
  return KERN_SUCCESS;
}

kern_return_t vm_machine_attribute(vm_map_t task, vm_address_t address,
                                   vm_size_t size, int attribute,
                                   vm_machine_attribute_val_t *value) {
  (void)task;
  (void)address;
  (void)size;
  (void)attribute;
  (void)value;
  return KERN_SUCCESS;
}

void *reallocf(void *pointer, size_t size) {
  void *result = realloc(pointer, size);
  if (result == NULL && size != 0)
    free(pointer);
  return result;
}

size_t strlcpy(char *destination, const char *source, size_t size) {
  size_t length = strlen(source);
  if (size != 0) {
    size_t copied = length < size - 1 ? length : size - 1;
    memcpy(destination, source, copied);
    destination[copied] = '\0';
  }
  return length;
}

size_t strlcat(char *destination, const char *source, size_t size) {
  size_t destination_length = strnlen(destination, size);
  if (destination_length == size)
    return size + strlen(source);
  return destination_length + strlcpy(destination + destination_length, source,
                                      size - destination_length);
}

void strmode(mode_t mode, char *result) {
  static const mode_t bits[] = {
      S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP,
      S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH,
  };
  static const char chars[] = "rwxrwxrwx";

  result[0] = S_ISDIR(mode) ? 'd' : S_ISLNK(mode) ? 'l' : '-';
  for (size_t i = 0; i < sizeof(bits) / sizeof(bits[0]); i++)
    result[i + 1] = mode & bits[i] ? chars[i] : '-';
  result[10] = ' ';
  result[11] = '\0';
}

int sysctlbyname(const char *name, void *old_value, size_t *old_size,
                 const void *new_value, size_t new_size) {
  (void)name;
  (void)old_value;
  (void)old_size;
  (void)new_value;
  (void)new_size;
  errno = ENOTSUP;
  return -1;
}

int sysctl(const int *name, unsigned int name_length, void *old_value,
           size_t *old_size, const void *new_value, size_t new_size) {
  (void)new_value;
  (void)new_size;
  if (name_length == 2 && name[0] == 1 && name[1] == 2 && old_value != NULL &&
      old_size != NULL) {
    static const char release[] = "24.0.0";
    if (*old_size < sizeof(release)) {
      errno = ENOMEM;
      return -1;
    }
    memcpy(old_value, release, sizeof(release));
    *old_size = sizeof(release) - 1;
    return 0;
  }
  errno = ENOTSUP;
  return -1;
}
