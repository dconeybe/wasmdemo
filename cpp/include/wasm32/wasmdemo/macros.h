#ifndef _WASMDEMO_MACROS_H_
#define _WASMDEMO_MACROS_H_

#define WASM_EXPORT(zz_export_name_zz) \
  __attribute__((export_name(zz_export_name_zz))) \
  extern "C"

#define WASM_IMPORT(zz_module_name_zz, zz_field_name_zz) \
  __attribute__((import_module(zz_module_name_zz))) \
  __attribute__((import_name(zz_field_name_zz)))

#endif // _WASMDEMO_MACROS_H_
