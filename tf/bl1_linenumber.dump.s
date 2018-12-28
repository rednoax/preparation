

   1 
   2 build/qemu/debug/bl1/bl1.elf:     file format elf32-littlearm
   3 build/qemu/debug/bl1/bl1.elf
   4 architecture: arm, flags 0x00000112:
   5 EXEC_P, HAS_SYMS, D_PAGED
   6 start address 0x00000000
   7 
   8 Program Header:
   9     LOAD off    0x00010000 vaddr 0x00000000 paddr 0x00000000 align 2**16
  10          filesz 0x000057f4 memsz 0x000057f4 flags r-x
  11     LOAD off    0x0001e000 vaddr 0x0e02e000 paddr 0x00005800 align 2**16
  12          filesz 0x0000008c memsz 0x0000008c flags rw-
  13     LOAD off    0x0001e08c vaddr 0x0e02e08c paddr 0x0e02e08c align 2**16
  14          filesz 0x00000000 memsz 0x00007f74 flags rw-
  15    STACK off    0x00000000 vaddr 0x00000000 paddr 0x00000000 align 2**4
  16          filesz 0x00000000 memsz 0x00000000 flags rwx
  17 private flags = 5000400: [Version5 EABI] [hard-float ABI]
  18 
  19 Sections:
  20 Idx Name          Size      VMA       LMA       File off  Algn
  21   0 .text         00004000  00000000  00000000  00010000  2**5
  22                   CONTENTS, ALLOC, LOAD, READONLY, CODE
  23   1 .rodata       000017f4  00004000  00004000  00014000  2**3
  24                   CONTENTS, ALLOC, LOAD, READONLY, DATA
  25   2 .data         0000008c  0e02e000  00005800  0001e000  2**4
  26                   CONTENTS, ALLOC, LOAD, DATA
  27   3 stacks        00001034  0e02e08c  0e02e08c  0001e08c  2**6
  28                   ALLOC
  29   4 .bss          00000348  0e02f0c0  0e02f0c0  0001e08c  2**5
  30                   ALLOC
  31   5 xlat_table    00006000  0e030000  0e030000  0001e08c  2**12
  32                   ALLOC
  33   6 coherent_ram  00000000  0e036000  0e036000  0001f000  2**12
  34                   CONTENTS
  35   7 .debug_info   0000c839  00000000  00000000  0001f000  2**0
  36                   CONTENTS, READONLY, DEBUGGING
  37   8 .debug_abbrev 00003329  00000000  00000000  0002b839  2**0
  38                   CONTENTS, READONLY, DEBUGGING
  39   9 .debug_aranges 00000a58  00000000  00000000  0002eb68  2**3
  40                   CONTENTS, READONLY, DEBUGGING
  41  10 .debug_line   00005dfc  00000000  00000000  0002f5c0  2**0
  42                   CONTENTS, READONLY, DEBUGGING
  43  11 .debug_str    000024dd  00000000  00000000  000353bc  2**0
  44                   CONTENTS, READONLY, DEBUGGING
  45  12 .comment      0000005e  00000000  00000000  00037899  2**0
  46                   CONTENTS, READONLY
  47  13 .ARM.attributes 00000040  00000000  00000000  000378f7  2**0
  48                   CONTENTS, READONLY
  49  14 .debug_loc    00007146  00000000  00000000  00037937  2**0
  50                   CONTENTS, READONLY, DEBUGGING
  51  15 .debug_ranges 00000e20  00000000  00000000  0003ea80  2**3
  52                   CONTENTS, READONLY, DEBUGGING
  53  16 .debug_frame  00001604  00000000  00000000  0003f8a0  2**2
  54                   CONTENTS, READONLY, DEBUGGING
  55 SYMBOL TABLE:
  56 00000000 l    d  .text  00000000 .text
  57 00004000 l    d  .rodata    00000000 .rodata
  58 0e02e000 l    d  .data  00000000 .data
  59 0e02e08c l    d  stacks 00000000 stacks
  60 0e02f0c0 l    d  .bss   00000000 .bss
  61 0e030000 l    d  xlat_table 00000000 xlat_table
  62 0e036000 l    d  coherent_ram   00000000 coherent_ram
  63 00000000 l    d  .debug_info    00000000 .debug_info
  64 00000000 l    d  .debug_abbrev  00000000 .debug_abbrev
  65 00000000 l    d  .debug_aranges 00000000 .debug_aranges
  66 00000000 l    d  .debug_line    00000000 .debug_line
  67 00000000 l    d  .debug_str 00000000 .debug_str
  68 00000000 l    d  .comment   00000000 .comment
  69 00000000 l    d  .ARM.attributes    00000000 .ARM.attributes
  70 00000000 l    d  .debug_loc 00000000 .debug_loc
  71 00000000 l    d  .debug_ranges  00000000 .debug_ranges
  72 00000000 l    d  .debug_frame   00000000 .debug_frame
  73 00000000 l    df *ABS*  00000000 ./build/qemu/debug/bl1/bl1_entrypoint.o
  74 000000b4 l       .text  00000000 do_primary_cold_boot
  75 00000114 l       .text  00000000 skip_mmu_off
  76 00000000 l    df *ABS*  00000000 io_semihosting.c
  77 000001dc l     F .text  00000008 device_type_sh
  78 000001e4 l     F .text  00000038 sh_dev_open
  79 0000021c l     F .text  0000003c sh_file_close
  80 00000258 l     F .text  00000088 sh_file_write
  81 000002e0 l     F .text  00000080 sh_file_read
  82 00000360 l     F .text  00000058 sh_file_len
  83 000003b8 l     F .text  00000040 sh_file_seek
  84 000003f8 l     F .text  00000058 sh_file_open
  85 00004048 l     O .rodata    00000004 sh_dev_connector
  86 0000404c l     O .rodata    00000024 sh_dev_funcs
  87 00004070 l     O .rodata    00000008 sh_dev_info
  88 00000000 l    df *ABS*  00000000 io_storage.c
  89 00000494 l     F .text  00000038 is_valid_dev
  90 000004cc l     F .text  00000020 is_valid_entity
  91 000004ec l     F .text  00000074 free_entity
  92 0e02f0c0 l     O .bss   00000004 dev_count
  93 0e02f0c4 l     O .bss   00000004 entity_count
  94 0e02f0c8 l     O .bss   00000010 entity_map
  95 0e02f0d8 l     O .bss   00000020 entity_pool
  96 00000000 l    df *ABS*  00000000 io_fip.c
  97 0000097c l     F .text  00000008 device_type_fip
  98 00000984 l     F .text  00000020 fip_dev_close
  99 000009a4 l     F .text  000000e0 fip_dev_init
 100 00000a84 l     F .text  00000030 fip_file_close
 101 00000ab4 l     F .text  0000013c fip_file_read
 102 00000bf0 l     F .text  00000038 fip_dev_open
 103 00000c28 l     F .text  0000004c fip_file_len
 104 00000c74 l     F .text  00000160 fip_file_open
 105 0e02f0f8 l     O .bss   00000004 backend_dev_handle
 106 0e02f0fc l     O .bss   00000004 backend_image_spec
 107 0e02f100 l     O .bss   00000030 current_file
 108 00004290 l     O .rodata    00000004 fip_dev_connector
 109 00004294 l     O .rodata    00000024 fip_dev_funcs
 110 000042b8 l     O .rodata    00000008 fip_dev_info
 111 00004490 l     O .rodata    00000010 uuid_null
 112 00000000 l    df *ABS*  00000000 io_memmap.c
 113 00000e18 l     F .text  00000008 device_type_memmap
 114 00000e20 l     F .text  00000008 memmap_dev_close
 115 00000e28 l     F .text  00000038 memmap_dev_open
 116 00000e60 l     F .text  0000004c memmap_block_len
 117 00000eac l     F .text  0000006c memmap_block_seek
 118 00000f18 l     F .text  00000044 memmap_block_close
 119 00000f5c l     F .text  0000009c memmap_block_write
 120 00000ff8 l     F .text  000000a4 memmap_block_read
 121 0000109c l     F .text  00000080 memmap_block_open
 122 0e02f130 l     O .bss   00000010 current_file
 123 00004544 l     O .rodata    00000004 memmap_dev_connector
 124 00004548 l     O .rodata    00000024 memmap_dev_funcs
 125 0000456c l     O .rodata    00000008 memmap_dev_info
 126 00000000 l    df *ABS*  00000000 semihosting.c
 127 00000000 l    df *ABS*  00000000 qemu_io_storage.c
 128 00001298 l     F .text  0000005c open_fip
 129 000012f4 l     F .text  0000005c open_memmap
 130 0e02f140 l     O .bss   00000004 fip_dev_con
 131 0e02f144 l     O .bss   00000004 fip_dev_handle
 132 0e02f148 l     O .bss   00000004 memmap_dev_con
 133 0e02f14c l     O .bss   00000004 memmap_dev_handle
 134 0e02f150 l     O .bss   00000004 sh_dev_con
 135 0e02f154 l     O .bss   00000004 sh_dev_handle
 136 0000458c l     O .rodata    00000010 bl2_uuid_spec
 137 0000459c l     O .rodata    00000010 bl31_uuid_spec
 138 000045ac l     O .rodata    00000010 bl32_extra1_uuid_spec
 139 000045bc l     O .rodata    00000010 bl32_extra2_uuid_spec
 140 000045cc l     O .rodata    00000010 bl32_uuid_spec
 141 000045dc l     O .rodata    00000010 bl33_uuid_spec
 142 000045ec l     O .rodata    00000008 fip_block_spec
 143 0000468c l     O .rodata    00000114 policies
 144 000047a0 l     O .rodata    000000b8 sh_file_spec
 145 00000000 l    df *ABS*  00000000 qemu_bl1_setup.c
 146 0e02f158 l     O .bss   00000008 bl1_tzram_layout
 147 00000000 l    df *ABS*  00000000 bl1_main.c
 148 00004ac4 l     O .rodata    00000010 bl1_svc_uid
 149 00000000 l    df *ABS*  00000000 bl1_arch_setup.c
 150 00000000 l    df *ABS*  00000000 bl1_context_mgmt.c
 151 0e02f160 l     O .bss   00000040 bl1_cpu_context
 152 0e02f1a0 l     O .bss   00000004 bl1_next_cpu_context_ptr
 153 0e02f1a4 l     O .bss   00000004 bl1_next_smc_context_ptr
 154 0e02f1a8 l     O .bss   00000120 bl1_smc_context
 155 00000000 l    df *ABS*  00000000 errata_report.c
 156 00000000 l    df *ABS*  00000000 context_mgmt.c
 157 00001b90 l     F .text  000000b8 cm_init_context_common
 158 00000000 l    df *ABS*  00000000 plat_bl1_common.c
 159 0e02e000 l     O .data  00000044 bl2_img_desc.6256
 160 00000000 l    df *ABS*  00000000 bl_common.c
 161 00000000 l    df *ABS*  00000000 tf_log.c
 162 0e02e044 l     O .data  00000004 max_log_level
 163 00000000 l    df *ABS*  00000000 tf_printf.c
 164 0000209c l     F .text  000000b0 unsigned_num_print
 165 00000000 l    df *ABS*  00000000 plat_bl_common.c
 166 00000000 l    df *ABS*  00000000 plat_log_common.c
 167 00004ec4 l     O .rodata    00000014 prefix_str
 168 00000000 l    df *ABS*  00000000 udivmoddi4.c
 169 00000000 l    df *ABS*  00000000 ctzdi2.c
 170 00000000 l    df *ABS*  00000000 assert.c
 171 00000000 l    df *ABS*  00000000 mem.c
 172 00000000 l    df *ABS*  00000000 putchar.c
 173 00000000 l    df *ABS*  00000000 strlen.c
 174 00000000 l    df *ABS*  00000000 qemu_common.c
 175 00004f20 l     O .rodata    00000078 plat_qemu_mmap
 176 00000000 l    df *ABS*  00000000 xlat_tables_arch.c
 177 00000000 l    df *ABS*  00000000 xlat_tables_internal.c
 178 00002984 l     F .text  00000168 xlat_desc_print.isra.1
 179 00002aec l     F .text  000001c8 xlat_tables_print_internal
 180 00002cb4 l     F .text  00000110 xlat_desc.isra.2
 181 00002dc4 l     F .text  000002d8 xlat_tables_map_region
 182 0e02f2e0 l     O .bss   00000020 tf_base_xlat_table
 183 0e02f300 l     O .bss   00000108 tf_mmap
 184 0e02e048 l     O .data  00000040 tf_xlat_ctx
 185 0e030000 l     O xlat_table 00006000 tf_xlat_tables
 186 000053b0 l     O .rodata    00000010 level_spacers
 187 00000000 l    df *ABS*  00000000 ./build/qemu/debug/bl1/semihosting_call.o
 188 00000000 l    df *ABS*  00000000 ./build/qemu/debug/bl1/plat_helpers.o
 189 00003808 l       .text  00000000 poll_mailbox
 190 00000000 l    df *ABS*  00000000 ./build/qemu/debug/bl1/cortex_a15.o
 191 00000000 l       *ABS*  00000000 CPU_MIDR
 192 00000004 l       *ABS*  00000000 CPU_RESET_FUNC
 193 00000008 l       *ABS*  00000000 CPU_ERRATA_FUNC
 194 0000000c l       *ABS*  00000000 CPU_OPS_SIZE
 195 0000385c l     F .text  00000014 cortex_a15_enable_smp
 196 00003870 l     F .text  00000008 check_errata_cve_2017_5715
 197 0000573b l       .rodata    00000000 cortex_a15_errata_cve_2017_5715_str
 198 00005749 l       .rodata    00000000 cortex_a15_cpu_str
 199 000038a8 l     F .text  00000004 cortex_a15_reset_func
 200 00000000 l    df *ABS*  00000000 ./build/qemu/debug/bl1/bl1_exceptions.o
 201 0000390c l     F .text  0000018c smc_handler
 202 000039c8 l       .text  00000000 skip_mmu_on
 203 00003a00 l       .text  00000000 skip_mmu_off
 204 00000000 l    df *ABS*  00000000 ./build/qemu/debug/bl1/cpu_helpers.o
 205 00000000 l       *ABS*  00000000 CPU_MIDR
 206 00000004 l       *ABS*  00000000 CPU_RESET_FUNC
 207 00000008 l       *ABS*  00000000 CPU_ERRATA_FUNC
 208 0000000c l       *ABS*  00000000 CPU_OPS_SIZE
 209 00003b04 l       .text  00000000 error_exit
 210 00000000 l    df *ABS*  00000000 ./build/qemu/debug/bl1/platform_up_stack.o
 211 0e02e0c0 l       stacks 00000000 platform_normal_stacks
 212 00000006 l       *ABS*  00000000 TZ_COUNT
 213 00000000 l    df *ABS*  00000000 ./build/qemu/debug/bl1/debug.o
 214 0000579e l       .rodata    00000000 panic_msg
 215 000057af l       .rodata    00000000 panic_end
 216 00003bf4 l     F .text  0000001c asm_print_str
 217 00003c10 l     F .text  00000034 asm_print_hex
 218 000057b2 l       .rodata    00000000 assert_msg1
 219 000057c0 l       .rodata    00000000 assert_msg2
 220 00000000 l    df *ABS*  00000000 ./build/qemu/debug/bl1/cache_helpers.o
 221 00003c7c l       .text  00000000 exit_loop_cimvac
 222 00003c68 l       .text  00000000 loop_cimvac
 223 00000000 l    df *ABS*  00000000 ./build/qemu/debug/bl1/misc_helpers.o
 224 00003cfc l       .text  00000000 m_loop4
 225 00003d14 l       .text  00000000 m_loop1
 226 00003d2c l       .text  00000000 m_end
 227 00003d38 l       .text  00000000 do_disable_mmu
 228 00000000 l    df *ABS*  00000000 ./build/qemu/debug/bl1/platform_helpers.o
 229 00000000 l    df *ABS*  00000000 ./build/qemu/debug/bl1/aeabi_uldivmod.o
 230 00000000 l    df *ABS*  00000000 ./build/qemu/debug/bl1/pl011_console.o
 231 0e02e088 l       .data  00000000 console_base
 232 00003dac l       .text  00000000 init_fail
 233 00003e30 l       .text  00000000 core_init_fail
 234 00003e74 l       .text  00000000 putc_error
 235 00003e98 l       .text  00000000 flush_error
 236 00000000 l    df *ABS*  00000000
 237 0000278c g     F .text  00000020 putchar
 238 00001c68 g     F .text  000000e4 cm_prepare_el3_exit
 239 000038ac g     F .text  00000060 bl1_aarch32_smc_handler
 240 00001350 g     F .text  000000d8 plat_qemu_io_setup
 241 0e036000 g       coherent_ram   00000000 __COHERENT_RAM_END__
 242 00003a98 g     F .text  00000034 reset_handler
 243 00003c80 g     F .text  00000064 zeromem
 244 0000401e g     O .rodata    00000017 version_string
 245 00001d54  w    F .text  00000004 bl1_plat_set_ep_info
 246 00003490 g     F .text  0000002c mmap_add_ctx
 247 00001f84 g     F .text  00000080 print_entry_point_info
 248 00001900 g     F .text  00000034 cm_get_context
 249 000034cc g     F .text  000000fc xlat_tables_print
 250 000018f0 g     F .text  00000010 smc_get_next_ctx
 251 00003b9c g     F .text  0000004c asm_assert
 252 00003b4c g     F .text  00000034 do_panic
 253 0000163c g     F .text  00000194 bl1_main
 254 0000274c g     F .text  00000040 memmove
 255 000027cc g     F .text  000000ac qemu_configure_mmu_secure
 256 00005800 g       *ABS*  00000000 __DATA_ROM_START__
 257 00001220 g     F .text  00000040 semihosting_file_write
 258 000037b8 g     F .text  00000008 semihosting_call
 259 000023a0  w    F .text  0000002c plat_log_get_prefix
 260 00003d5c  w    F .text  00000004 plat_report_exception
 261 00004000 g     O .rodata    0000001e build_message
 262 00003784 g     F .text  0000000c init_xlat_tables
 263 00003ce4 g     F .text  0000004c memcpy4
 264 0e036000 g       xlat_table 00000000 __BL1_RAM_END__
 265 00002730 g     F .text  0000001c memcpy
 266 00003c80 g     F .text  00000064 zero_normalmem
 267 00003d74 g     F .text  00000024 __aeabi_uldivmod
 268 00003e3c g     F .text  00000040 console_core_putc
 269 000017d0 g     F .text  00000020 bl1_print_next_bl_ep_info
 270 00003834 g     F .text  00000004 platform_mem_init
 271 000034bc g     F .text  00000010 mmap_add
 272 00003d70  w    F .text  00000004 plat_panic_handler
 273 0e02f0c0 g       stacks 00000000 __STACKS_END__
 274 000023cc g     F .text  000002e4 __udivmoddi4
 275 0000008c g       *ABS*  00000000 __DATA_SIZE__
 276 00000848 g     F .text  00000060 io_size
 277 00001638  w    F .text  00000004 bl1_init_bl2_mem_layout
 278 0e02e000 g       .data  00000000 __BL1_RAM_START__
 279 000005a8 g     F .text  00000054 io_dev_open
 280 00003b3c  w    F .text  0000000c plat_set_my_stack
 281 0000309c g     F .text  00000074 print_mmap
 282 00000348 g       *ABS*  00000000 __BSS_SIZE__
 283 00003dd4 g     F .text  00000064 console_core_init
 284 00003d60  w    F .text  00000008 plat_crash_console_flush
 285 00002390  w    F .text  00000008 plat_error_handler
 286 00003d98 g     F .text  00000018 console_init
 287 00003db4 g     F .text  0000000c console_putc
 288 000037f8 g     F .text  00000034 plat_secondary_cold_boot_setup
 289 000015bc g     F .text  0000007c bl1_calc_bl2_mem_layout
 290 00001160 g     F .text  00000024 semihosting_file_open
 291 00003838 g     F .text  00000018 plat_crash_console_init
 292 00003c44 g     F .text  0000003c flush_dcache_range
 293 000037c8 g     F .text  00000010 plat_qemu_calc_core_pos
 294 00003d68  w    F .text  00000004 plat_reset_handler
 295 00003ad0 g     F .text  00000038 get_cpu_ops_ptr
 296 00001184 g     F .text  00000030 semihosting_file_seek
 297 00001c48 g     F .text  00000020 cm_init_my_context
 298 000018a8 g     F .text  00000004 bl1_arch_setup
 299 00001dd0 g     F .text  000001b4 load_auth_image
 300 00001d60  w    F .text  0000000c bl1_plat_get_image_desc
 301 000015b8 g     F .text  00000004 bl1_platform_setup
 302 000057f4 g       .rodata    00000000 __RODATA_END__
 303 00001d6c  w    F .text  00000064 bl1_plat_handle_post_image_load
 304 0e02e08c g       .data  00000000 __DATA_RAM_END__
 305 0000127c g     F .text  0000001c semihosting_file_length
 306 00004000 g       .rodata    00000000 __RODATA_START__
 307 000057e8 g       .rodata    00000000 __CPU_OPS_START__
 308 00003ea0 g       .text  00000000 bl1_vector_table
 309 000018ac g     F .text  00000044 smc_set_next_ctx
 310 00002704 g     F .text  0000002c memcmp
 311 00003b14 g     F .text  00000010 cpu_get_rev_var
 312 000037d8 g     F .text  0000001c plat_is_my_cpu_primary
 313 00001934 g     F .text  00000034 cm_set_next_context
 314 0e02f408 g       .bss   00000000 __BSS_END__
 315 000057f4 g       .rodata    00000000 __CPU_OPS_END__
 316 0000382c g     F .text  00000008 plat_get_my_entrypoint
 317 000007d8 g     F .text  00000070 io_seek
 318 0000091c g     F .text  00000060 io_close
 319 00003d6c  w    F .text  00000004 bl1_plat_prepare_exit
 320 000026b0 g     F .text  00000030 __ctzdi2
 321 00001550 g     F .text  00000068 bl1_plat_arch_setup
 322 00002188 g     F .text  000001e0 tf_vprintf
 323 00003d50 g     F .text  00000008 disable_mmu_icache_secure
 324 0e036000 g       coherent_ram   00000000 __COHERENT_RAM_START__
 325 000057e8 g       .rodata    00000000 __PARSER_LIB_DESCS_END__
 326 00001ac4 g     F .text  000000cc errata_print_msg
 327 00001864 g     F .text  00000044 bl1_smc_wrapper
 328 00003790 g     F .text  00000028 enable_mmu_secure
 329 0000588c g       *ABS*  00000000 __BL1_ROM_END__
 330 00000000 g       *ABS*  00000000 __COHERENT_RAM_UNALIGNED_SIZE__
 331 00003878  w    F .text  00000028 cortex_a15_errata_report
 332 00002878 g     F .text  0000000c xlat_arch_get_max_supported_pa
 333 00000450 g     F .text  00000044 register_io_dev_sh
 334 00001968 g     F .text  0000015c bl1_prepare_next_image
 335 00004000 g       .text  00000000 __TEXT_END__
 336 0000214c g     F .text  0000003c tf_string_print
 337 00003d34 g     F .text  0000001c disable_mmu_secure
 338 00001d4c  w    F .text  00000008 bl1_plat_get_next_image_id
 339 00002368 g     F .text  00000028 tf_printf
 340 00003e7c g     F .text  00000024 console_core_flush
 341 00003dc4 g     F .text  0000000c console_flush
 342 000005fc g     F .text  00000068 io_dev_init
 343 000008a8 g     F .text  00000074 io_read
 344 00001510 g     F .text  0000000c bl1_plat_sec_mem_layout
 345 00003b24 g     F .text  00000018 print_errata_status
 346 00001260 g     F .text  0000001c semihosting_file_close
 347 0e02f0c0 g       .bss   00000000 __BSS_START__
 348 0e02e000 g       .data  00000000 __DATA_RAM_START__
 349 00003110 g     F .text  0000034c mmap_add_region_ctx
 350 00002398  w    F .text  00000008 plat_try_next_boot_source
 351 00003850 g     F .text  0000000c plat_crash_console_putc
 352 00002890 g     F .text  000000f4 enable_mmu_arch
 353 000006c4 g     F .text  00000114 io_open
 354 00002884 g     F .text  0000000c is_mmu_enabled_ctx
 355 00001428 g     F .text  000000e8 plat_get_image_source
 356 00002004 g     F .text  00000098 tf_log
 357 00000560 g     F .text  00000048 io_register_device
 358 0e036000 g       coherent_ram   00000000 __COHERENT_RAM_END_UNALIGNED__
 359 0e02e08c g       stacks 00000000 __STACKS_START__
 360 000017f0 g     F .text  00000074 bl1_smc_handler
 361 00001d58  w    F .text  00000008 bl1_plat_handle_pre_image_load
 362 000035c8 g     F .text  000001bc init_xlat_tables_ctx
 363 000037c0 g     F .text  00000008 plat_my_core_pos
 364 0000111c g     F .text  00000044 register_io_dev_memmap
 365 000027ac g     F .text  00000020 strlen
 366 00003b88 g     F .text  00000014 report_exception
 367 00000664 g     F .text  00000060 io_dev_close
 368 000026e0 g     F .text  00000024 __assert
 369 0000151c g     F .text  00000034 bl1_early_platform_setup
 370 00000dd4 g     F .text  00000044 register_io_dev_fip
 371 00000000 g     F .text  000001ac bl1_entrypoint
 372 0000345c g     F .text  00000034 mmap_add_region
 373 00000000 g       .text  00000000 __TEXT_START__
 374 000057e8 g       .rodata    00000000 __PARSER_LIB_DESCS_START__
 375 000011b4 g     F .text  0000006c semihosting_file_read
 376 
 377 
 378 
 379 Disassembly of section .text:
 380 
 381 00000000 <bl1_entrypoint>:
 382        0:   ee110f11    mrc 15, 0, r0, cr1, cr1, {0}
 383        4:   e3100001    tst r0, #1
 384        8:   0a000002    beq 18 <CPU_OPS_SIZE+0xc>
 385        c:   e59f0198    ldr r0, [pc, #408]  ; 1ac <skip_mmu_off+0x98>
 386       10:   e3a010a2    mov r1, #162    ; 0xa2
 387       14:   ea000ee0    b   3b9c <asm_assert>
 388       18:   e59f0190    ldr r0, [pc, #400]  ; 1b0 <skip_mmu_off+0x9c>
 389       1c:   ee010f10    mcr 15, 0, r0, cr1, cr0, {0}
 390       20:   f57ff06f    isb sy
 391       24:   f1020016    cps #22
 392       28:   f57ff06f    isb sy
 393       2c:   eb000dfe    bl  382c <plat_get_my_entrypoint>
 394       30:   e3500000    cmp r0, #0
 395       34:   112fff10    bxne    r0
 396       38:   e59f0174    ldr r0, [pc, #372]  ; 1b4 <skip_mmu_off+0xa0>
 397       3c:   ee0c0f10    mcr 15, 0, r0, cr12, cr0, {0}
 398       40:   ee0c0f30    mcr 15, 0, r0, cr12, cr0, {1}
 399       44:   f57ff06f    isb sy
 400       48:   eb000e92    bl  3a98 <reset_handler>
 401       4c:   e59f1164    ldr r1, [pc, #356]  ; 1b8 <skip_mmu_off+0xa4>
 402       50:   ee110f10    mrc 15, 0, r0, cr1, cr0, {0}
 403       54:   e1800001    orr r0, r0, r1
 404       58:   ee010f10    mcr 15, 0, r0, cr1, cr0, {0}
 405       5c:   f57ff06f    isb sy
 406       60:   e3a00c02    mov r0, #512    ; 0x200
 407       64:   ee010f11    mcr 15, 0, r0, cr1, cr1, {0}
 408       68:   f1080100    cpsie   a
 409       6c:   f57ff06f    isb sy
 410       70:   ee110f51    mrc 15, 0, r0, cr1, cr1, {2}
 411       74:   e2000807    and r0, r0, #458752 ; 0x70000
 412       78:   e3800b03    orr r0, r0, #3072   ; 0xc00
 413       7c:   ee010f51    mcr 15, 0, r0, cr1, cr1, {2}
 414       80:   f57ff06f    isb sy
 415       84:   e3a0060f    mov r0, #15728640   ; 0xf00000
 416       88:   ee010f50    mcr 15, 0, r0, cr1, cr0, {2}
 417       8c:   f57ff06f    isb sy
 418       90:   e59f0124    ldr r0, [pc, #292]  ; 1bc <skip_mmu_off+0xa8>
 419       94:   eee80a10    vmsr    fpexc, r0
 420       98:   f57ff06f    isb sy
 421       9c:   eb000dcd    bl  37d8 <plat_is_my_cpu_primary>
 422       a0:   e3500000    cmp r0, #0
 423       a4:   1a000002    bne b4 <do_primary_cold_boot>
 424       a8:   eb000dd2    bl  37f8 <plat_secondary_cold_boot_setup>
 425       ac:   eb000f2f    bl  3d70 <plat_panic_handler>
 426       b0:   e320f000    nop {0}
 427 
 428 000000b4 <do_primary_cold_boot>:
 429       b4:   eb000dde    bl  3834 <platform_mem_init>
 430       b8:   e59f0100    ldr r0, [pc, #256]  ; 1c0 <skip_mmu_off+0xac>
 431       bc:   e59f1100    ldr r1, [pc, #256]  ; 1c4 <skip_mmu_off+0xb0>
 432       c0:   eb000eee    bl  3c80 <zero_normalmem>
 433       c4:   e59f00fc    ldr r0, [pc, #252]  ; 1c8 <skip_mmu_off+0xb4>
 434       c8:   e59f10fc    ldr r1, [pc, #252]  ; 1cc <skip_mmu_off+0xb8>
 435       cc:   eb000eeb    bl  3c80 <zero_normalmem>
 436       d0:   e59f00f8    ldr r0, [pc, #248]  ; 1d0 <skip_mmu_off+0xbc>
 437       d4:   e59f10f8    ldr r1, [pc, #248]  ; 1d4 <skip_mmu_off+0xc0>
 438       d8:   e59f20f8    ldr r2, [pc, #248]  ; 1d8 <skip_mmu_off+0xc4>
 439       dc:   eb000f00    bl  3ce4 <memcpy4>
 440       e0:   eb000e95    bl  3b3c <plat_set_my_stack>
 441       e4:   eb00050c    bl  151c <bl1_early_platform_setup>
 442       e8:   eb000518    bl  1550 <bl1_plat_arch_setup>
 443       ec:   eb000552    bl  163c <bl1_main>
 444       f0:   eb0005fe    bl  18f0 <smc_get_next_ctx>
 445       f4:   e1a04000    mov r4, r0
 446       f8:   e5945084    ldr r5, [r4, #132]  ; 0x84
 447       fc:   e3150001    tst r5, #1
 448      100:   1a000003    bne 114 <skip_mmu_off>
 449      104:   eb000f11    bl  3d50 <disable_mmu_icache_secure>
 450      108:   ee080f17    mcr 15, 0, r0, cr8, cr7, {0}
 451      10c:   f57ff04f    dsb sy
 452      110:   f57ff06f    isb sy
 453 
 454 00000114 <skip_mmu_off>:
 455      114:   e1a00004    mov r0, r4
 456      118:   e580d07c    str sp, [r0, #124]  ; 0x7c
 457      11c:   e1a0d000    mov sp, r0
 458      120:   e5901084    ldr r1, [r0, #132]  ; 0x84
 459      124:   ee011f11    mcr 15, 0, r1, cr1, cr1, {0}
 460      128:   f57ff06f    isb sy
 461      12c:   e5901088    ldr r1, [r0, #136]  ; 0x88
 462      130:   ee091f1c    mcr 15, 0, r1, cr9, cr12, {0}
 463      134:   e2801034    add r1, r0, #52 ; 0x34
 464      138:   ee114f11    mrc 15, 0, r4, cr1, cr1, {0}
 465      13c:   e3c42001    bic r2, r4, #1
 466      140:   ee012f11    mcr 15, 0, r2, cr1, cr1, {0}
 467      144:   f57ff06f    isb sy
 468      148:   f102001f    cps #31
 469      14c:   e8b16000    ldm r1!, {sp, lr}
 470      150:   f1020012    cps #18
 471      154:   e8b16004    ldm r1!, {r2, sp, lr}
 472      158:   e16ff002    msr SPSR_fsxc, r2
 473      15c:   f1020011    cps #17
 474      160:   e8b16004    ldm r1!, {r2, sp, lr}
 475      164:   e16ff002    msr SPSR_fsxc, r2
 476      168:   f1020013    cps #19
 477      16c:   e8b16004    ldm r1!, {r2, sp, lr}
 478      170:   e16ff002    msr SPSR_fsxc, r2
 479      174:   f1020017    cps #23
 480      178:   e8b16004    ldm r1!, {r2, sp, lr}
 481      17c:   e16ff002    msr SPSR_fsxc, r2
 482      180:   f102001b    cps #27
 483      184:   e8b16004    ldm r1!, {r2, sp, lr}
 484      188:   e16ff002    msr SPSR_fsxc, r2
 485      18c:   f1020016    cps #22
 486      190:   e8b10004    ldm r1!, {r2}
 487      194:   e16ff002    msr SPSR_fsxc, r2
 488      198:   ee014f11    mcr 15, 0, r4, cr1, cr1, {0}
 489      19c:   f57ff06f    isb sy
 490      1a0:   e590e080    ldr lr, [r0, #128]  ; 0x80
 491      1a4:   e8901fff    ldm r0, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
 492      1a8:   e1b0f00e    movs    pc, lr
 493      1ac:   00005754    .word   0x00005754
 494      1b0:   00c50038    .word   0x00c50038
 495      1b4:   00003ea0    .word   0x00003ea0
 496      1b8:   00001002    .word   0x00001002
 497      1bc:   40000700    .word   0x40000700
 498      1c0:   0e02f0c0    .word   0x0e02f0c0
 499      1c4:   00000348    .word   0x00000348
 500      1c8:   0e036000    .word   0x0e036000
 501      1cc:   00000000    .word   0x00000000
 502      1d0:   0e02e000    .word   0x0e02e000
 503      1d4:   00005800    .word   0x00005800
 504      1d8:   0000008c    .word   0x0000008c
 505 
 506 000001dc <device_type_sh>:
 507      1dc:   e3a00001    mov r0, #1
 508      1e0:   e12fff1e    bx  lr
 509 
 510 000001e4 <sh_dev_open>:
 511      1e4:   e3510000    cmp r1, #0
 512      1e8:   1a000004    bne 200 <sh_dev_open+0x1c>
 513      1ec:   e92d4010    push    {r4, lr}
 514      1f0:   e3a01040    mov r1, #64 ; 0x40
 515      1f4:   e59f2014    ldr r2, [pc, #20]   ; 210 <sh_dev_open+0x2c>
 516      1f8:   e59f0014    ldr r0, [pc, #20]   ; 214 <sh_dev_open+0x30>
 517      1fc:   eb000937    bl  26e0 <__assert>
 518      200:   e59f3010    ldr r3, [pc, #16]   ; 218 <sh_dev_open+0x34>
 519      204:   e3a00000    mov r0, #0
 520      208:   e5813000    str r3, [r1]
 521      20c:   e12fff1e    bx  lr
 522      210:   00004078    .word   0x00004078
 523      214:   00004089    .word   0x00004089
 524      218:   00004070    .word   0x00004070
 525 
 526 0000021c <sh_file_close>:
 527      21c:   e3500000    cmp r0, #0
 528      220:   e92d4010    push    {r4, lr}
 529      224:   1a000003    bne 238 <sh_file_close+0x1c>
 530      228:   e59f2020    ldr r2, [pc, #32]   ; 250 <sh_file_close+0x34>
 531      22c:   e3a010b4    mov r1, #180    ; 0xb4
 532      230:   e59f001c    ldr r0, [pc, #28]   ; 254 <sh_file_close+0x38>
 533      234:   eb000929    bl  26e0 <__assert>
 534      238:   e5900004    ldr r0, [r0, #4]
 535      23c:   eb000407    bl  1260 <semihosting_file_close>
 536      240:   e3500000    cmp r0, #0
 537      244:   b3e00001    mvnlt   r0, #1
 538      248:   a3a00000    movge   r0, #0
 539      24c:   e8bd8010    pop {r4, pc}
 540      250:   000040a5    .word   0x000040a5
 541      254:   00004089    .word   0x00004089
 542 
 543 00000258 <sh_file_write>:
 544      258:   e92d4037    push    {r0, r1, r2, r4, r5, lr}
 545      25c:   e3500000    cmp r0, #0
 546      260:   03a010a0    moveq   r1, #160    ; 0xa0
 547      264:   e58d2004    str r2, [sp, #4]
 548      268:   059f2060    ldreq   r2, [pc, #96]   ; 2d0 <sh_file_write+0x78>
 549      26c:   0a000003    beq 280 <sh_file_write+0x28>
 550      270:   e3510000    cmp r1, #0
 551      274:   1a000003    bne 288 <sh_file_write+0x30>
 552      278:   e59f2054    ldr r2, [pc, #84]   ; 2d4 <sh_file_write+0x7c>
 553      27c:   e3a010a1    mov r1, #161    ; 0xa1
 554      280:   e59f0050    ldr r0, [pc, #80]   ; 2d8 <sh_file_write+0x80>
 555      284:   eb000915    bl  26e0 <__assert>
 556      288:   e3530000    cmp r3, #0
 557      28c:   e1a05003    mov r5, r3
 558      290:   059f2044    ldreq   r2, [pc, #68]   ; 2dc <sh_file_write+0x84>
 559      294:   03a010a2    moveq   r1, #162    ; 0xa2
 560      298:   0afffff8    beq 280 <sh_file_write+0x28>
 561      29c:   e1a04002    mov r4, r2
 562      2a0:   e1a02001    mov r2, r1
 563      2a4:   e5900004    ldr r0, [r0, #4]
 564      2a8:   e28d1004    add r1, sp, #4
 565      2ac:   eb0003db    bl  1220 <semihosting_file_write>
 566      2b0:   e59d3004    ldr r3, [sp, #4]
 567      2b4:   e3500000    cmp r0, #0
 568      2b8:   13e00001    mvnne   r0, #1
 569      2bc:   03a00000    moveq   r0, #0
 570      2c0:   e0444003    sub r4, r4, r3
 571      2c4:   e5854000    str r4, [r5]
 572      2c8:   e28dd00c    add sp, sp, #12
 573      2cc:   e8bd8030    pop {r4, r5, pc}
 574      2d0:   000040a5    .word   0x000040a5
 575      2d4:   000040e9    .word   0x000040e9
 576      2d8:   00004089    .word   0x00004089
 577      2dc:   00004103    .word   0x00004103
 578 
 579 000002e0 <sh_file_read>:
 580      2e0:   e92d4013    push    {r0, r1, r4, lr}
 581      2e4:   e3500000    cmp r0, #0
 582      2e8:   03a01087    moveq   r1, #135    ; 0x87
 583      2ec:   e58d2004    str r2, [sp, #4]
 584      2f0:   059f2058    ldreq   r2, [pc, #88]   ; 350 <__BSS_SIZE__+0x8>
 585      2f4:   0a000003    beq 308 <sh_file_read+0x28>
 586      2f8:   e3510000    cmp r1, #0
 587      2fc:   1a000003    bne 310 <sh_file_read+0x30>
 588      300:   e59f204c    ldr r2, [pc, #76]   ; 354 <__BSS_SIZE__+0xc>
 589      304:   e3a01088    mov r1, #136    ; 0x88
 590      308:   e59f0048    ldr r0, [pc, #72]   ; 358 <__BSS_SIZE__+0x10>
 591      30c:   eb0008f3    bl  26e0 <__assert>
 592      310:   e3530000    cmp r3, #0
 593      314:   e1a04003    mov r4, r3
 594      318:   059f203c    ldreq   r2, [pc, #60]   ; 35c <__BSS_SIZE__+0x14>
 595      31c:   03a01089    moveq   r1, #137    ; 0x89
 596      320:   0afffff8    beq 308 <sh_file_read+0x28>
 597      324:   e1a02001    mov r2, r1
 598      328:   e28d1004    add r1, sp, #4
 599      32c:   e5900004    ldr r0, [r0, #4]
 600      330:   eb00039f    bl  11b4 <semihosting_file_read>
 601      334:   e3500000    cmp r0, #0
 602      338:   a59d3004    ldrge   r3, [sp, #4]
 603      33c:   a3a00000    movge   r0, #0
 604      340:   b3e00001    mvnlt   r0, #1
 605      344:   a5843000    strge   r3, [r4]
 606      348:   e28dd008    add sp, sp, #8
 607      34c:   e8bd8010    pop {r4, pc}
 608      350:   000040a5    .word   0x000040a5
 609      354:   000040e9    .word   0x000040e9
 610      358:   00004089    .word   0x00004089
 611      35c:   000040d5    .word   0x000040d5
 612 
 613 00000360 <sh_file_len>:
 614      360:   e3500000    cmp r0, #0
 615      364:   e92d4010    push    {r4, lr}
 616      368:   059f203c    ldreq   r2, [pc, #60]   ; 3ac <sh_file_len+0x4c>
 617      36c:   03a0106f    moveq   r1, #111    ; 0x6f
 618      370:   0a000004    beq 388 <sh_file_len+0x28>
 619      374:   e3510000    cmp r1, #0
 620      378:   e1a04001    mov r4, r1
 621      37c:   1a000003    bne 390 <sh_file_len+0x30>
 622      380:   e59f2028    ldr r2, [pc, #40]   ; 3b0 <sh_file_len+0x50>
 623      384:   e3a01070    mov r1, #112    ; 0x70
 624      388:   e59f0024    ldr r0, [pc, #36]   ; 3b4 <sh_file_len+0x54>
 625      38c:   eb0008d3    bl  26e0 <__assert>
 626      390:   e5900004    ldr r0, [r0, #4]
 627      394:   eb0003b8    bl  127c <semihosting_file_length>
 628      398:   e3500000    cmp r0, #0
 629      39c:   a5840000    strge   r0, [r4]
 630      3a0:   a3a00000    movge   r0, #0
 631      3a4:   b3e00001    mvnlt   r0, #1
 632      3a8:   e8bd8010    pop {r4, pc}
 633      3ac:   000040a5    .word   0x000040a5
 634      3b0:   000040b4    .word   0x000040b4
 635      3b4:   00004089    .word   0x00004089
 636 
 637 000003b8 <sh_file_seek>:
 638      3b8:   e3500000    cmp r0, #0
 639      3bc:   e92d4010    push    {r4, lr}
 640      3c0:   1a000003    bne 3d4 <sh_file_seek+0x1c>
 641      3c4:   e59f2024    ldr r2, [pc, #36]   ; 3f0 <sh_file_seek+0x38>
 642      3c8:   e3a01060    mov r1, #96 ; 0x60
 643      3cc:   e59f0020    ldr r0, [pc, #32]   ; 3f4 <sh_file_seek+0x3c>
 644      3d0:   eb0008c2    bl  26e0 <__assert>
 645      3d4:   e1a01002    mov r1, r2
 646      3d8:   e5900004    ldr r0, [r0, #4]
 647      3dc:   eb000368    bl  1184 <semihosting_file_seek>
 648      3e0:   e3500000    cmp r0, #0
 649      3e4:   13e00001    mvnne   r0, #1
 650      3e8:   03a00000    moveq   r0, #0
 651      3ec:   e8bd8010    pop {r4, pc}
 652      3f0:   000040a5    .word   0x000040a5
 653      3f4:   00004089    .word   0x00004089
 654 
 655 000003f8 <sh_file_open>:
 656      3f8:   e2513000    subs    r3, r1, #0
 657      3fc:   e92d4010    push    {r4, lr}
 658      400:   059f203c    ldreq   r2, [pc, #60]   ; 444 <sh_file_open+0x4c>
 659      404:   03a0104e    moveq   r1, #78 ; 0x4e
 660      408:   0a000004    beq 420 <sh_file_open+0x28>
 661      40c:   e3520000    cmp r2, #0
 662      410:   e1a04002    mov r4, r2
 663      414:   1a000003    bne 428 <sh_file_open+0x30>
 664      418:   e59f2028    ldr r2, [pc, #40]   ; 448 <sh_file_open+0x50>
 665      41c:   e3a0104f    mov r1, #79 ; 0x4f
 666      420:   e59f0024    ldr r0, [pc, #36]   ; 44c <sh_file_open+0x54>
 667      424:   eb0008ad    bl  26e0 <__assert>
 668      428:   e1c300d0    ldrd    r0, [r3]
 669      42c:   eb00034b    bl  1160 <semihosting_file_open>
 670      430:   e3500000    cmp r0, #0
 671      434:   c5840004    strgt   r0, [r4, #4]
 672      438:   c3a00000    movgt   r0, #0
 673      43c:   d3e00001    mvnle   r0, #1
 674      440:   e8bd8010    pop {r4, pc}
 675      444:   000040c3    .word   0x000040c3
 676      448:   000040a5    .word   0x000040a5
 677      44c:   00004089    .word   0x00004089
 678 
 679 00000450 <register_io_dev_sh>:
 680      450:   e92d4010    push    {r4, lr}
 681      454:   e2504000    subs    r4, r0, #0
 682      458:   1a000003    bne 46c <register_io_dev_sh+0x1c>
 683      45c:   e59f2020    ldr r2, [pc, #32]   ; 484 <register_io_dev_sh+0x34>
 684      460:   e3a010c4    mov r1, #196    ; 0xc4
 685      464:   e59f001c    ldr r0, [pc, #28]   ; 488 <register_io_dev_sh+0x38>
 686      468:   eb00089c    bl  26e0 <__assert>
 687      46c:   e59f0018    ldr r0, [pc, #24]   ; 48c <register_io_dev_sh+0x3c>
 688      470:   eb00003a    bl  560 <io_register_device>
 689      474:   e3500000    cmp r0, #0
 690      478:   059f3010    ldreq   r3, [pc, #16]   ; 490 <register_io_dev_sh+0x40>
 691      47c:   05843000    streq   r3, [r4]
 692      480:   e8bd8010    pop {r4, pc}
 693      484:   00004035    .word   0x00004035
 694      488:   00004089    .word   0x00004089
 695      48c:   00004070    .word   0x00004070
 696      490:   00004048    .word   0x00004048
 697 
 698 00000494 <is_valid_dev>:
 699      494:   e3500000    cmp r0, #0
 700      498:   012fff1e    bxeq    lr
 701      49c:   e5900000    ldr r0, [r0]
 702      4a0:   e3500000    cmp r0, #0
 703      4a4:   012fff1e    bxeq    lr
 704      4a8:   e5900000    ldr r0, [r0]
 705      4ac:   e3500000    cmp r0, #0
 706      4b0:   012fff1e    bxeq    lr
 707      4b4:   e92d4010    push    {r4, lr}
 708      4b8:   e12fff30    blx r0
 709      4bc:   e3500005    cmp r0, #5
 710      4c0:   83a00000    movhi   r0, #0
 711      4c4:   93a00001    movls   r0, #1
 712      4c8:   e8bd8010    pop {r4, pc}
 713 
 714 000004cc <is_valid_entity>:
 715      4cc:   e3500000    cmp r0, #0
 716      4d0:   012fff1e    bxeq    lr
 717      4d4:   e92d4010    push    {r4, lr}
 718      4d8:   e5900000    ldr r0, [r0]
 719      4dc:   ebffffec    bl  494 <is_valid_dev>
 720      4e0:   e2900000    adds    r0, r0, #0
 721      4e4:   13a00001    movne   r0, #1
 722      4e8:   e8bd8010    pop {r4, pc}
 723 
 724 000004ec <free_entity>:
 725      4ec:   e3500000    cmp r0, #0
 726      4f0:   1a000004    bne 508 <free_entity+0x1c>
 727      4f4:   e92d4010    push    {r4, lr}
 728      4f8:   e3a01083    mov r1, #131    ; 0x83
 729      4fc:   e59f204c    ldr r2, [pc, #76]   ; 550 <free_entity+0x64>
 730      500:   e59f004c    ldr r0, [pc, #76]   ; 554 <free_entity+0x68>
 731      504:   eb000875    bl  26e0 <__assert>
 732      508:   e59f2048    ldr r2, [pc, #72]   ; 558 <free_entity+0x6c>
 733      50c:   e3a03000    mov r3, #0
 734      510:   e1a01002    mov r1, r2
 735      514:   e492c004    ldr ip, [r2], #4
 736      518:   e150000c    cmp r0, ip
 737      51c:   0a000004    beq 534 <free_entity+0x48>
 738      520:   e2833001    add r3, r3, #1
 739      524:   e3530004    cmp r3, #4
 740      528:   1afffff9    bne 514 <free_entity+0x28>
 741      52c:   e3e00001    mvn r0, #1
 742      530:   e12fff1e    bx  lr
 743      534:   e59f2020    ldr r2, [pc, #32]   ; 55c <free_entity+0x70>
 744      538:   e3a00000    mov r0, #0
 745      53c:   e7810103    str r0, [r1, r3, lsl #2]
 746      540:   e5923000    ldr r3, [r2]
 747      544:   e2433001    sub r3, r3, #1
 748      548:   e5823000    str r3, [r2]
 749      54c:   e12fff1e    bx  lr
 750      550:   000040a5    .word   0x000040a5
 751      554:   0000411a    .word   0x0000411a
 752      558:   0e02f0c8    .word   0x0e02f0c8
 753      55c:   0e02f0c4    .word   0x0e02f0c4
 754 
 755 00000560 <io_register_device>:
 756      560:   e3500000    cmp r0, #0
 757      564:   1a000004    bne 57c <io_register_device+0x1c>
 758      568:   e92d4010    push    {r4, lr}
 759      56c:   e3a01095    mov r1, #149    ; 0x95
 760      570:   e59f2024    ldr r2, [pc, #36]   ; 59c <io_register_device+0x3c>
 761      574:   e59f0024    ldr r0, [pc, #36]   ; 5a0 <io_register_device+0x40>
 762      578:   eb000858    bl  26e0 <__assert>
 763      57c:   e59f2020    ldr r2, [pc, #32]   ; 5a4 <io_register_device+0x44>
 764      580:   e5923000    ldr r3, [r2]
 765      584:   e3530002    cmp r3, #2
 766      588:   92833001    addls   r3, r3, #1
 767      58c:   93a00000    movls   r0, #0
 768      590:   83e0000b    mvnhi   r0, #11
 769      594:   95823000    strls   r3, [r2]
 770      598:   e12fff1e    bx  lr
 771      59c:   00004078    .word   0x00004078
 772      5a0:   0000411a    .word   0x0000411a
 773      5a4:   0e02f0c0    .word   0x0e02f0c0
 774 
 775 000005a8 <io_dev_open>:
 776      5a8:   e1a03000    mov r3, r0
 777      5ac:   e1a00001    mov r0, r1
 778      5b0:   e2521000    subs    r1, r2, #0
 779      5b4:   e92d4010    push    {r4, lr}
 780      5b8:   059f2030    ldreq   r2, [pc, #48]   ; 5f0 <io_dev_open+0x48>
 781      5bc:   03a010a6    moveq   r1, #166    ; 0xa6
 782      5c0:   0a000008    beq 5e8 <io_dev_open+0x40>
 783      5c4:   e3530000    cmp r3, #0
 784      5c8:   0a000004    beq 5e0 <io_dev_open+0x38>
 785      5cc:   e5933000    ldr r3, [r3]
 786      5d0:   e3530000    cmp r3, #0
 787      5d4:   0a000001    beq 5e0 <io_dev_open+0x38>
 788      5d8:   e8bd4010    pop {r4, lr}
 789      5dc:   e12fff13    bx  r3
 790      5e0:   e59f200c    ldr r2, [pc, #12]   ; 5f4 <io_dev_open+0x4c>
 791      5e4:   e3a0104e    mov r1, #78 ; 0x4e
 792      5e8:   e59f0008    ldr r0, [pc, #8]    ; 5f8 <io_dev_open+0x50>
 793      5ec:   eb00083b    bl  26e0 <__assert>
 794      5f0:   00004181    .word   0x00004181
 795      5f4:   00004190    .word   0x00004190
 796      5f8:   0000411a    .word   0x0000411a
 797 
 798 000005fc <io_dev_init>:
 799      5fc:   e92d4070    push    {r4, r5, r6, lr}
 800      600:   e2504000    subs    r4, r0, #0
 801      604:   059f204c    ldreq   r2, [pc, #76]   ; 658 <io_dev_init+0x5c>
 802      608:   03a010b2    moveq   r1, #178    ; 0xb2
 803      60c:   0a000005    beq 628 <io_dev_init+0x2c>
 804      610:   e1a05001    mov r5, r1
 805      614:   ebffff9e    bl  494 <is_valid_dev>
 806      618:   e3500000    cmp r0, #0
 807      61c:   1a000003    bne 630 <io_dev_init+0x34>
 808      620:   e59f2034    ldr r2, [pc, #52]   ; 65c <io_dev_init+0x60>
 809      624:   e3a010b3    mov r1, #179    ; 0xb3
 810      628:   e59f0030    ldr r0, [pc, #48]   ; 660 <io_dev_init+0x64>
 811      62c:   eb00082b    bl  26e0 <__assert>
 812      630:   e5943000    ldr r3, [r4]
 813      634:   e593301c    ldr r3, [r3, #28]
 814      638:   e3530000    cmp r3, #0
 815      63c:   0a000003    beq 650 <io_dev_init+0x54>
 816      640:   e1a01005    mov r1, r5
 817      644:   e1a00004    mov r0, r4
 818      648:   e8bd4070    pop {r4, r5, r6, lr}
 819      64c:   e12fff13    bx  r3
 820      650:   e1a00003    mov r0, r3
 821      654:   e8bd8070    pop {r4, r5, r6, pc}
 822      658:   0000414a    .word   0x0000414a
 823      65c:   00004168    .word   0x00004168
 824      660:   0000411a    .word   0x0000411a
 825 
 826 00000664 <io_dev_close>:
 827      664:   e92d4010    push    {r4, lr}
 828      668:   e2504000    subs    r4, r0, #0
 829      66c:   059f2044    ldreq   r2, [pc, #68]   ; 6b8 <io_dev_close+0x54>
 830      670:   03a010c6    moveq   r1, #198    ; 0xc6
 831      674:   0a000004    beq 68c <io_dev_close+0x28>
 832      678:   ebffff85    bl  494 <is_valid_dev>
 833      67c:   e3500000    cmp r0, #0
 834      680:   1a000003    bne 694 <io_dev_close+0x30>
 835      684:   e59f2030    ldr r2, [pc, #48]   ; 6bc <io_dev_close+0x58>
 836      688:   e3a010c7    mov r1, #199    ; 0xc7
 837      68c:   e59f002c    ldr r0, [pc, #44]   ; 6c0 <io_dev_close+0x5c>
 838      690:   eb000812    bl  26e0 <__assert>
 839      694:   e5943000    ldr r3, [r4]
 840      698:   e5933020    ldr r3, [r3, #32]
 841      69c:   e3530000    cmp r3, #0
 842      6a0:   0a000002    beq 6b0 <io_dev_close+0x4c>
 843      6a4:   e1a00004    mov r0, r4
 844      6a8:   e8bd4010    pop {r4, lr}
 845      6ac:   e12fff13    bx  r3
 846      6b0:   e1a00003    mov r0, r3
 847      6b4:   e8bd8010    pop {r4, pc}
 848      6b8:   0000414a    .word   0x0000414a
 849      6bc:   00004168    .word   0x00004168
 850      6c0:   0000411a    .word   0x0000411a
 851 
 852 000006c4 <io_open>:
 853      6c4:   e92d47f0    push    {r4, r5, r6, r7, r8, r9, sl, lr}
 854      6c8:   e1a05002    mov r5, r2
 855      6cc:   e16f2f12    clz r2, r2
 856      6d0:   e3510000    cmp r1, #0
 857      6d4:   e1a022a2    lsr r2, r2, #5
 858      6d8:   03a04001    moveq   r4, #1
 859      6dc:   11a04002    movne   r4, r2
 860      6e0:   e3540000    cmp r4, #0
 861      6e4:   159f20cc    ldrne   r2, [pc, #204]  ; 7b8 <io_open+0xf4>
 862      6e8:   13a010db    movne   r1, #219    ; 0xdb
 863      6ec:   1a000006    bne 70c <io_open+0x48>
 864      6f0:   e1a06000    mov r6, r0
 865      6f4:   e1a07001    mov r7, r1
 866      6f8:   ebffff65    bl  494 <is_valid_dev>
 867      6fc:   e3500000    cmp r0, #0
 868      700:   1a000003    bne 714 <io_open+0x50>
 869      704:   e59f20b0    ldr r2, [pc, #176]  ; 7bc <io_open+0xf8>
 870      708:   e3a010dc    mov r1, #220    ; 0xdc
 871      70c:   e59f00ac    ldr r0, [pc, #172]  ; 7c0 <io_open+0xfc>
 872      710:   eb0007f2    bl  26e0 <__assert>
 873      714:   e59f30a8    ldr r3, [pc, #168]  ; 7c4 <io_open+0x100>
 874      718:   e5932000    ldr r2, [r3]
 875      71c:   e3520003    cmp r2, #3
 876      720:   8a000016    bhi 780 <io_open+0xbc>
 877      724:   e59f109c    ldr r1, [pc, #156]  ; 7c8 <io_open+0x104>
 878      728:   e1a00001    mov r0, r1
 879      72c:   e491c004    ldr ip, [r1], #4
 880      730:   e35c0000    cmp ip, #0
 881      734:   0a000013    beq 788 <io_open+0xc4>
 882      738:   e2844001    add r4, r4, #1
 883      73c:   e3540004    cmp r4, #4
 884      740:   1afffff9    bne 72c <io_open+0x68>
 885      744:   e59f2080    ldr r2, [pc, #128]  ; 7cc <io_open+0x108>
 886      748:   e3a01075    mov r1, #117    ; 0x75
 887      74c:   eaffffee    b   70c <io_open+0x48>
 888      750:   e1a01007    mov r1, r7
 889      754:   e1a02008    mov r2, r8
 890      758:   e1a00006    mov r0, r6
 891      75c:   e12fff33    blx r3
 892      760:   e2507000    subs    r7, r0, #0
 893      764:   07896184    streq   r6, [r9, r4, lsl #3]
 894      768:   05858000    streq   r8, [r5]
 895      76c:   0a000001    beq 778 <io_open+0xb4>
 896      770:   e1a00008    mov r0, r8
 897      774:   ebffff5c    bl  4ec <free_entity>
 898      778:   e1a00007    mov r0, r7
 899      77c:   e8bd87f0    pop {r4, r5, r6, r7, r8, r9, sl, pc}
 900      780:   e3e0700b    mvn r7, #11
 901      784:   eafffffb    b   778 <io_open+0xb4>
 902      788:   e2822001    add r2, r2, #1
 903      78c:   e59f903c    ldr r9, [pc, #60]   ; 7d0 <io_open+0x10c>
 904      790:   e5832000    str r2, [r3]
 905      794:   e5963000    ldr r3, [r6]
 906      798:   e0898184    add r8, r9, r4, lsl #3
 907      79c:   e5933004    ldr r3, [r3, #4]
 908      7a0:   e7808104    str r8, [r0, r4, lsl #2]
 909      7a4:   e3530000    cmp r3, #0
 910      7a8:   1affffe8    bne 750 <io_open+0x8c>
 911      7ac:   e59f2020    ldr r2, [pc, #32]   ; 7d4 <io_open+0x110>
 912      7b0:   e3a010e4    mov r1, #228    ; 0xe4
 913      7b4:   eaffffd4    b   70c <io_open+0x48>
 914      7b8:   000041b0    .word   0x000041b0
 915      7bc:   00004168    .word   0x00004168
 916      7c0:   0000411a    .word   0x0000411a
 917      7c4:   0e02f0c4    .word   0x0e02f0c4
 918      7c8:   0e02f0c8    .word   0x0e02f0c8
 919      7cc:   00004662    .word   0x00004662
 920      7d0:   0e02f0d8    .word   0x0e02f0d8
 921      7d4:   000041de    .word   0x000041de
 922 
 923 000007d8 <io_seek>:
 924      7d8:   e92d4070    push    {r4, r5, r6, lr}
 925      7dc:   e1a04000    mov r4, r0
 926      7e0:   e1a05001    mov r5, r1
 927      7e4:   e1a06002    mov r6, r2
 928      7e8:   ebffff37    bl  4cc <is_valid_entity>
 929      7ec:   e3500000    cmp r0, #0
 930      7f0:   0a000002    beq 800 <io_seek+0x28>
 931      7f4:   e2453001    sub r3, r5, #1
 932      7f8:   e3530002    cmp r3, #2
 933      7fc:   9a000003    bls 810 <io_seek+0x38>
 934      800:   e59f2038    ldr r2, [pc, #56]   ; 840 <io_seek+0x68>
 935      804:   e3a010f5    mov r1, #245    ; 0xf5
 936      808:   e59f0034    ldr r0, [pc, #52]   ; 844 <io_seek+0x6c>
 937      80c:   eb0007b3    bl  26e0 <__assert>
 938      810:   e5943000    ldr r3, [r4]
 939      814:   e5933000    ldr r3, [r3]
 940      818:   e5933008    ldr r3, [r3, #8]
 941      81c:   e3530000    cmp r3, #0
 942      820:   0a000004    beq 838 <io_seek+0x60>
 943      824:   e1a02006    mov r2, r6
 944      828:   e1a01005    mov r1, r5
 945      82c:   e1a00004    mov r0, r4
 946      830:   e8bd4070    pop {r4, r5, r6, lr}
 947      834:   e12fff13    bx  r3
 948      838:   e3e00012    mvn r0, #18
 949      83c:   e8bd8070    pop {r4, r5, r6, pc}
 950      840:   0000422e    .word   0x0000422e
 951      844:   0000411a    .word   0x0000411a
 952 
 953 00000848 <io_size>:
 954      848:   e92d4070    push    {r4, r5, r6, lr}
 955      84c:   e1a05001    mov r5, r1
 956      850:   e1a04000    mov r4, r0
 957      854:   ebffff1c    bl  4cc <is_valid_entity>
 958      858:   e3500000    cmp r0, #0
 959      85c:   13550000    cmpne   r5, #0
 960      860:   1a000003    bne 874 <io_size+0x2c>
 961      864:   e59f2034    ldr r2, [pc, #52]   ; 8a0 <io_size+0x58>
 962      868:   e3001106    movw    r1, #262    ; 0x106
 963      86c:   e59f0030    ldr r0, [pc, #48]   ; 8a4 <io_size+0x5c>
 964      870:   eb00079a    bl  26e0 <__assert>
 965      874:   e5943000    ldr r3, [r4]
 966      878:   e5933000    ldr r3, [r3]
 967      87c:   e593300c    ldr r3, [r3, #12]
 968      880:   e3530000    cmp r3, #0
 969      884:   0a000003    beq 898 <io_size+0x50>
 970      888:   e1a01005    mov r1, r5
 971      88c:   e1a00004    mov r0, r4
 972      890:   e8bd4070    pop {r4, r5, r6, lr}
 973      894:   e12fff13    bx  r3
 974      898:   e3e00012    mvn r0, #18
 975      89c:   e8bd8070    pop {r4, r5, r6, pc}
 976      8a0:   00004262    .word   0x00004262
 977      8a4:   0000411a    .word   0x0000411a
 978 
 979 000008a8 <io_read>:
 980      8a8:   e92d41f0    push    {r4, r5, r6, r7, r8, lr}
 981      8ac:   e1a05001    mov r5, r1
 982      8b0:   e1a04000    mov r4, r0
 983      8b4:   e1a06002    mov r6, r2
 984      8b8:   e1a07003    mov r7, r3
 985      8bc:   ebffff02    bl  4cc <is_valid_entity>
 986      8c0:   e3500000    cmp r0, #0
 987      8c4:   13550000    cmpne   r5, #0
 988      8c8:   1a000003    bne 8dc <io_read+0x34>
 989      8cc:   e59f2040    ldr r2, [pc, #64]   ; 914 <io_read+0x6c>
 990      8d0:   e300111a    movw    r1, #282    ; 0x11a
 991      8d4:   e59f003c    ldr r0, [pc, #60]   ; 918 <io_read+0x70>
 992      8d8:   eb000780    bl  26e0 <__assert>
 993      8dc:   e5943000    ldr r3, [r4]
 994      8e0:   e5933000    ldr r3, [r3]
 995      8e4:   e593e010    ldr lr, [r3, #16]
 996      8e8:   e35e0000    cmp lr, #0
 997      8ec:   0a000006    beq 90c <io_read+0x64>
 998      8f0:   e1a03007    mov r3, r7
 999      8f4:   e1a02006    mov r2, r6
1000      8f8:   e1a01005    mov r1, r5
1001      8fc:   e1a00004    mov r0, r4
1002      900:   e1a0c00e    mov ip, lr
1003      904:   e8bd41f0    pop {r4, r5, r6, r7, r8, lr}
1004      908:   e12fff1c    bx  ip
1005      90c:   e3e00012    mvn r0, #18
1006      910:   e8bd81f0    pop {r4, r5, r6, r7, r8, pc}
1007      914:   000041f7    .word   0x000041f7
1008      918:   0000411a    .word   0x0000411a
1009 
1010 0000091c <io_close>:
1011      91c:   e92d4070    push    {r4, r5, r6, lr}
1012      920:   e1a05000    mov r5, r0
1013      924:   ebfffee8    bl  4cc <is_valid_entity>
1014      928:   e3500000    cmp r0, #0
1015      92c:   1a000003    bne 940 <io_close+0x24>
1016      930:   e59f203c    ldr r2, [pc, #60]   ; 974 <io_close+0x58>
1017      934:   e3001141    movw    r1, #321    ; 0x141
1018      938:   e59f0038    ldr r0, [pc, #56]   ; 978 <io_close+0x5c>
1019      93c:   eb000767    bl  26e0 <__assert>
1020      940:   e5953000    ldr r3, [r5]
1021      944:   e5933000    ldr r3, [r3]
1022      948:   e5933018    ldr r3, [r3, #24]
1023      94c:   e3530000    cmp r3, #0
1024      950:   01a04003    moveq   r4, r3
1025      954:   0a000002    beq 964 <io_close+0x48>
1026      958:   e1a00005    mov r0, r5
1027      95c:   e12fff33    blx r3
1028      960:   e1a04000    mov r4, r0
1029      964:   e1a00005    mov r0, r5
1030      968:   ebfffedf    bl  4ec <free_entity>
1031      96c:   e1a00004    mov r0, r4
1032      970:   e8bd8070    pop {r4, r5, r6, pc}
1033      974:   00004132    .word   0x00004132
1034      978:   0000411a    .word   0x0000411a
1035 
1036 0000097c <device_type_fip>:
1037      97c:   e3a00004    mov r0, #4
1038      980:   e12fff1e    bx  lr
1039 
1040 00000984 <fip_dev_close>:
1041      984:   e59f3010    ldr r3, [pc, #16]   ; 99c <fip_dev_close+0x18>
1042      988:   e3a00000    mov r0, #0
1043      98c:   e5830000    str r0, [r3]
1044      990:   e59f3008    ldr r3, [pc, #8]    ; 9a0 <fip_dev_close+0x1c>
1045      994:   e5830000    str r0, [r3]
1046      998:   e12fff1e    bx  lr
1047      99c:   0e02f0f8    .word   0x0e02f0f8
1048      9a0:   0e02f0fc    .word   0x0e02f0fc
1049 
1050 000009a4 <fip_dev_init>:
1051      9a4:   e92d4070    push    {r4, r5, r6, lr}
1052      9a8:   e1a04001    mov r4, r1
1053      9ac:   e59f60b4    ldr r6, [pc, #180]  ; a68 <fip_dev_init+0xc4>
1054      9b0:   e24dd018    sub sp, sp, #24
1055      9b4:   e1a00004    mov r0, r4
1056      9b8:   e59f50ac    ldr r5, [pc, #172]  ; a6c <fip_dev_init+0xc8>
1057      9bc:   e1a02006    mov r2, r6
1058      9c0:   e1a01005    mov r1, r5
1059      9c4:   eb000297    bl  1428 <plat_get_image_source>
1060      9c8:   e2502000    subs    r2, r0, #0
1061      9cc:   11a01004    movne   r1, r4
1062      9d0:   159f0098    ldrne   r0, [pc, #152]  ; a70 <fip_dev_init+0xcc>
1063      9d4:   1a000007    bne 9f8 <fip_dev_init+0x54>
1064      9d8:   e1a0200d    mov r2, sp
1065      9dc:   e5950000    ldr r0, [r5]
1066      9e0:   e5961000    ldr r1, [r6]
1067      9e4:   ebffff36    bl  6c4 <io_open>
1068      9e8:   e2502000    subs    r2, r0, #0
1069      9ec:   0a000006    beq a0c <fip_dev_init+0x68>
1070      9f0:   e59f007c    ldr r0, [pc, #124]  ; a74 <fip_dev_init+0xd0>
1071      9f4:   e1a01004    mov r1, r4
1072      9f8:   e3e04001    mvn r4, #1
1073      9fc:   eb000580    bl  2004 <tf_log>
1074      a00:   e1a00004    mov r0, r4
1075      a04:   e28dd018    add sp, sp, #24
1076      a08:   e8bd8070    pop {r4, r5, r6, pc}
1077      a0c:   e28d3004    add r3, sp, #4
1078      a10:   e3a02010    mov r2, #16
1079      a14:   e59d0000    ldr r0, [sp]
1080      a18:   e28d1008    add r1, sp, #8
1081      a1c:   ebffffa1    bl  8a8 <io_read>
1082      a20:   e2504000    subs    r4, r0, #0
1083      a24:   1a000008    bne a4c <fip_dev_init+0xa8>
1084      a28:   e59f3048    ldr r3, [pc, #72]   ; a78 <fip_dev_init+0xd4>
1085      a2c:   e59d2008    ldr r2, [sp, #8]
1086      a30:   e1520003    cmp r2, r3
1087      a34:   1a000007    bne a58 <fip_dev_init+0xb4>
1088      a38:   e59d300c    ldr r3, [sp, #12]
1089      a3c:   e3530000    cmp r3, #0
1090      a40:   0a000004    beq a58 <fip_dev_init+0xb4>
1091      a44:   e59f0030    ldr r0, [pc, #48]   ; a7c <fip_dev_init+0xd8>
1092      a48:   eb00056d    bl  2004 <tf_log>
1093      a4c:   e59d0000    ldr r0, [sp]
1094      a50:   ebffffb1    bl  91c <io_close>
1095      a54:   eaffffe9    b   a00 <fip_dev_init+0x5c>
1096      a58:   e59f0020    ldr r0, [pc, #32]   ; a80 <fip_dev_init+0xdc>
1097      a5c:   e3e04001    mvn r4, #1
1098      a60:   eb000567    bl  2004 <tf_log>
1099      a64:   eafffff8    b   a4c <fip_dev_init+0xa8>
1100      a68:   0e02f0fc    .word   0x0e02f0fc
1101      a6c:   0e02f0f8    .word   0x0e02f0f8
1102      a70:   000042c0    .word   0x000042c0
1103      a74:   000042f1    .word   0x000042f1
1104      a78:   aa640001    .word   0xaa640001
1105      a7c:   00004315    .word   0x00004315
1106      a80:   0000432c    .word   0x0000432c
1107 
1108 00000a84 <fip_file_close>:
1109      a84:   e92d4010    push    {r4, lr}
1110      a88:   e1a04000    mov r4, r0
1111      a8c:   e59f001c    ldr r0, [pc, #28]   ; ab0 <fip_file_close+0x2c>
1112      a90:   e1c021d8    ldrd    r2, [r0, #24]
1113      a94:   e1923003    orrs    r3, r2, r3
1114      a98:   0a000001    beq aa4 <fip_file_close+0x20>
1115      a9c:   e3a01030    mov r1, #48 ; 0x30
1116      aa0:   eb000c76    bl  3c80 <zero_normalmem>
1117      aa4:   e3a00000    mov r0, #0
1118      aa8:   e5840004    str r0, [r4, #4]
1119      aac:   e8bd8010    pop {r4, pc}
1120      ab0:   0e02f100    .word   0x0e02f100
1121 
1122 00000ab4 <fip_file_read>:
1123      ab4:   e92d40f7    push    {r0, r1, r2, r4, r5, r6, r7, lr}
1124      ab8:   e2505000    subs    r5, r0, #0
1125      abc:   059f2104    ldreq   r2, [pc, #260]  ; bc8 <fip_file_read+0x114>
1126      ac0:   03001112    movweq  r1, #274    ; 0x112
1127      ac4:   0a000004    beq adc <fip_file_read+0x28>
1128      ac8:   e3510000    cmp r1, #0
1129      acc:   e1a04001    mov r4, r1
1130      ad0:   1a000003    bne ae4 <fip_file_read+0x30>
1131      ad4:   e59f20f0    ldr r2, [pc, #240]  ; bcc <fip_file_read+0x118>
1132      ad8:   e3001113    movw    r1, #275    ; 0x113
1133      adc:   e59f00ec    ldr r0, [pc, #236]  ; bd0 <fip_file_read+0x11c>
1134      ae0:   eb0006fe    bl  26e0 <__assert>
1135      ae4:   e3530000    cmp r3, #0
1136      ae8:   e1a06003    mov r6, r3
1137      aec:   059f20e0    ldreq   r2, [pc, #224]  ; bd4 <fip_file_read+0x120>
1138      af0:   03a01f45    moveq   r1, #276    ; 0x114
1139      af4:   0afffff8    beq adc <fip_file_read+0x28>
1140      af8:   e5953004    ldr r3, [r5, #4]
1141      afc:   e3530000    cmp r3, #0
1142      b00:   059f20d0    ldreq   r2, [pc, #208]  ; bd8 <fip_file_read+0x124>
1143      b04:   03001115    movweq  r1, #277    ; 0x115
1144      b08:   0afffff3    beq adc <fip_file_read+0x28>
1145      b0c:   e59f30c8    ldr r3, [pc, #200]  ; bdc <fip_file_read+0x128>
1146      b10:   e1a07002    mov r7, r2
1147      b14:   e28d2004    add r2, sp, #4
1148      b18:   e5931000    ldr r1, [r3]
1149      b1c:   e59f30bc    ldr r3, [pc, #188]  ; be0 <fip_file_read+0x12c>
1150      b20:   e5930000    ldr r0, [r3]
1151      b24:   ebfffee6    bl  6c4 <io_open>
1152      b28:   e2501000    subs    r1, r0, #0
1153      b2c:   0a000005    beq b48 <fip_file_read+0x94>
1154      b30:   e59f00ac    ldr r0, [pc, #172]  ; be4 <fip_file_read+0x130>
1155      b34:   e3e04001    mvn r4, #1
1156      b38:   eb000531    bl  2004 <tf_log>
1157      b3c:   e1a00004    mov r0, r4
1158      b40:   e28dd00c    add sp, sp, #12
1159      b44:   e8bd80f0    pop {r4, r5, r6, r7, pc}
1160      b48:   e5955004    ldr r5, [r5, #4]
1161      b4c:   e3a01001    mov r1, #1
1162      b50:   e59d0004    ldr r0, [sp, #4]
1163      b54:   e5953000    ldr r3, [r5]
1164      b58:   e5952018    ldr r2, [r5, #24]
1165      b5c:   e0822003    add r2, r2, r3
1166      b60:   ebffff1c    bl  7d8 <io_seek>
1167      b64:   e3500000    cmp r0, #0
1168      b68:   0a000005    beq b84 <fip_file_read+0xd0>
1169      b6c:   e59f0074    ldr r0, [pc, #116]  ; be8 <fip_file_read+0x134>
1170      b70:   eb000523    bl  2004 <tf_log>
1171      b74:   e3e04001    mvn r4, #1
1172      b78:   e59d0004    ldr r0, [sp, #4]
1173      b7c:   ebffff66    bl  91c <io_close>
1174      b80:   eaffffed    b   b3c <fip_file_read+0x88>
1175      b84:   e1a01004    mov r1, r4
1176      b88:   e1a0300d    mov r3, sp
1177      b8c:   e59d0004    ldr r0, [sp, #4]
1178      b90:   e1a02007    mov r2, r7
1179      b94:   ebffff43    bl  8a8 <io_read>
1180      b98:   e2504000    subs    r4, r0, #0
1181      b9c:   0a000003    beq bb0 <fip_file_read+0xfc>
1182      ba0:   e1a01004    mov r1, r4
1183      ba4:   e59f0040    ldr r0, [pc, #64]   ; bec <fip_file_read+0x138>
1184      ba8:   eb000515    bl  2004 <tf_log>
1185      bac:   eafffff0    b   b74 <fip_file_read+0xc0>
1186      bb0:   e59d2000    ldr r2, [sp]
1187      bb4:   e5862000    str r2, [r6]
1188      bb8:   e5953000    ldr r3, [r5]
1189      bbc:   e0833002    add r3, r3, r2
1190      bc0:   e5853000    str r3, [r5]
1191      bc4:   eaffffeb    b   b78 <fip_file_read+0xc4>
1192      bc8:   000040a5    .word   0x000040a5
1193      bcc:   000040e9    .word   0x000040e9
1194      bd0:   00004403    .word   0x00004403
1195      bd4:   000040d5    .word   0x000040d5
1196      bd8:   00004417    .word   0x00004417
1197      bdc:   0e02f0fc    .word   0x0e02f0fc
1198      be0:   0e02f0f8    .word   0x0e02f0f8
1199      be4:   00004437    .word   0x00004437
1200      be8:   00004451    .word   0x00004451
1201      bec:   00004471    .word   0x00004471
1202 
1203 00000bf0 <fip_dev_open>:
1204      bf0:   e3510000    cmp r1, #0
1205      bf4:   1a000004    bne c0c <fip_dev_open+0x1c>
1206      bf8:   e92d4010    push    {r4, lr}
1207      bfc:   e3a0106b    mov r1, #107    ; 0x6b
1208      c00:   e59f2014    ldr r2, [pc, #20]   ; c1c <fip_dev_open+0x2c>
1209      c04:   e59f0014    ldr r0, [pc, #20]   ; c20 <fip_dev_open+0x30>
1210      c08:   eb0006b4    bl  26e0 <__assert>
1211      c0c:   e59f3010    ldr r3, [pc, #16]   ; c24 <fip_dev_open+0x34>
1212      c10:   e3a00000    mov r0, #0
1213      c14:   e5813000    str r3, [r1]
1214      c18:   e12fff1e    bx  lr
1215      c1c:   00004078    .word   0x00004078
1216      c20:   00004403    .word   0x00004403
1217      c24:   000042b8    .word   0x000042b8
1218 
1219 00000c28 <fip_file_len>:
1220      c28:   e3500000    cmp r0, #0
1221      c2c:   e92d4010    push    {r4, lr}
1222      c30:   059f2030    ldreq   r2, [pc, #48]   ; c68 <fip_file_len+0x40>
1223      c34:   03a010ff    moveq   r1, #255    ; 0xff
1224      c38:   0a000003    beq c4c <fip_file_len+0x24>
1225      c3c:   e3510000    cmp r1, #0
1226      c40:   1a000003    bne c54 <fip_file_len+0x2c>
1227      c44:   e59f2020    ldr r2, [pc, #32]   ; c6c <fip_file_len+0x44>
1228      c48:   e3a01c01    mov r1, #256    ; 0x100
1229      c4c:   e59f001c    ldr r0, [pc, #28]   ; c70 <fip_file_len+0x48>
1230      c50:   eb0006a2    bl  26e0 <__assert>
1231      c54:   e5903004    ldr r3, [r0, #4]
1232      c58:   e3a00000    mov r0, #0
1233      c5c:   e5933020    ldr r3, [r3, #32]
1234      c60:   e5813000    str r3, [r1]
1235      c64:   e8bd8010    pop {r4, pc}
1236      c68:   000040a5    .word   0x000040a5
1237      c6c:   000040b4    .word   0x000040b4
1238      c70:   00004403    .word   0x00004403
1239 
1240 00000c74 <fip_file_open>:
1241      c74:   e92d43f7    push    {r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
1242      c78:   e2518000    subs    r8, r1, #0
1243      c7c:   059f2124    ldreq   r2, [pc, #292]  ; da8 <fip_file_open+0x134>
1244      c80:   03a010b6    moveq   r1, #182    ; 0xb6
1245      c84:   0a000004    beq c9c <fip_file_open+0x28>
1246      c88:   e3520000    cmp r2, #0
1247      c8c:   e1a07002    mov r7, r2
1248      c90:   1a000003    bne ca4 <fip_file_open+0x30>
1249      c94:   e59f2110    ldr r2, [pc, #272]  ; dac <fip_file_open+0x138>
1250      c98:   e3a010b7    mov r1, #183    ; 0xb7
1251      c9c:   e59f010c    ldr r0, [pc, #268]  ; db0 <fip_file_open+0x13c>
1252      ca0:   eb00068e    bl  26e0 <__assert>
1253      ca4:   e59f5108    ldr r5, [pc, #264]  ; db4 <fip_file_open+0x140>
1254      ca8:   e1c521d8    ldrd    r2, [r5, #24]
1255      cac:   e1923003    orrs    r3, r2, r3
1256      cb0:   0a000005    beq ccc <fip_file_open+0x58>
1257      cb4:   e59f00fc    ldr r0, [pc, #252]  ; db8 <fip_file_open+0x144>
1258      cb8:   e3e0400b    mvn r4, #11
1259      cbc:   eb0004d0    bl  2004 <tf_log>
1260      cc0:   e1a00004    mov r0, r4
1261      cc4:   e28dd00c    add sp, sp, #12
1262      cc8:   e8bd83f0    pop {r4, r5, r6, r7, r8, r9, pc}
1263      ccc:   e59f30e8    ldr r3, [pc, #232]  ; dbc <fip_file_open+0x148>
1264      cd0:   e1a0200d    mov r2, sp
1265      cd4:   e5931000    ldr r1, [r3]
1266      cd8:   e59f30e0    ldr r3, [pc, #224]  ; dc0 <fip_file_open+0x14c>
1267      cdc:   e5930000    ldr r0, [r3]
1268      ce0:   ebfffe77    bl  6c4 <io_open>
1269      ce4:   e2501000    subs    r1, r0, #0
1270      ce8:   0a000003    beq cfc <fip_file_open+0x88>
1271      cec:   e59f00d0    ldr r0, [pc, #208]  ; dc4 <fip_file_open+0x150>
1272      cf0:   e3e04001    mvn r4, #1
1273      cf4:   eb0004c2    bl  2004 <tf_log>
1274      cf8:   eafffff0    b   cc0 <fip_file_open+0x4c>
1275      cfc:   e3a02010    mov r2, #16
1276      d00:   e3a01001    mov r1, #1
1277      d04:   e59d0000    ldr r0, [sp]
1278      d08:   ebfffeb2    bl  7d8 <io_seek>
1279      d0c:   e3500000    cmp r0, #0
1280      d10:   02856008    addeq   r6, r5, #8
1281      d14:   059f90ac    ldreq   r9, [pc, #172]  ; dc8 <fip_file_open+0x154>
1282      d18:   0a000003    beq d2c <fip_file_open+0xb8>
1283      d1c:   e59f00a8    ldr r0, [pc, #168]  ; dcc <fip_file_open+0x158>
1284      d20:   eb0004b7    bl  2004 <tf_log>
1285      d24:   e3e04001    mvn r4, #1
1286      d28:   ea000012    b   d78 <fip_file_open+0x104>
1287      d2c:   e28d3004    add r3, sp, #4
1288      d30:   e3a02028    mov r2, #40 ; 0x28
1289      d34:   e59d0000    ldr r0, [sp]
1290      d38:   e1a01006    mov r1, r6
1291      d3c:   ebfffed9    bl  8a8 <io_read>
1292      d40:   e2504000    subs    r4, r0, #0
1293      d44:   1a000008    bne d6c <fip_file_open+0xf8>
1294      d48:   e3a02010    mov r2, #16
1295      d4c:   e1a01008    mov r1, r8
1296      d50:   e1a00006    mov r0, r6
1297      d54:   eb00066a    bl  2704 <memcmp>
1298      d58:   e2504000    subs    r4, r0, #0
1299      d5c:   1a000008    bne d84 <fip_file_open+0x110>
1300      d60:   e5854000    str r4, [r5]
1301      d64:   e5875004    str r5, [r7, #4]
1302      d68:   ea000002    b   d78 <fip_file_open+0x104>
1303      d6c:   e1a01004    mov r1, r4
1304      d70:   e59f0058    ldr r0, [pc, #88]   ; dd0 <fip_file_open+0x15c>
1305      d74:   eb0004a2    bl  2004 <tf_log>
1306      d78:   e59d0000    ldr r0, [sp]
1307      d7c:   ebfffee6    bl  91c <io_close>
1308      d80:   eaffffce    b   cc0 <fip_file_open+0x4c>
1309      d84:   e3a02010    mov r2, #16
1310      d88:   e1a01009    mov r1, r9
1311      d8c:   e1a00006    mov r0, r6
1312      d90:   eb00065b    bl  2704 <memcmp>
1313      d94:   e3500000    cmp r0, #0
1314      d98:   1affffe3    bne d2c <fip_file_open+0xb8>
1315      d9c:   f2c00010    vmov.i32    d16, #0 ; 0x00000000
1316      da0:   edc50b06    vstr    d16, [r5, #24]
1317      da4:   eaffffde    b   d24 <fip_file_open+0xb0>
1318      da8:   0000435a    .word   0x0000435a
1319      dac:   000040a5    .word   0x000040a5
1320      db0:   00004403    .word   0x00004403
1321      db4:   0e02f100    .word   0x0e02f100
1322      db8:   0000436c    .word   0x0000436c
1323      dbc:   0e02f0fc    .word   0x0e02f0fc
1324      dc0:   0e02f0f8    .word   0x0e02f0f8
1325      dc4:   0000439c    .word   0x0000439c
1326      dc8:   00004490    .word   0x00004490
1327      dcc:   000043c9    .word   0x000043c9
1328      dd0:   000043e9    .word   0x000043e9
1329 
1330 00000dd4 <register_io_dev_fip>:
1331      dd4:   e92d4010    push    {r4, lr}
1332      dd8:   e2504000    subs    r4, r0, #0
1333      ddc:   1a000003    bne df0 <register_io_dev_fip+0x1c>
1334      de0:   e59f2020    ldr r2, [pc, #32]   ; e08 <register_io_dev_fip+0x34>
1335      de4:   e3001156    movw    r1, #342    ; 0x156
1336      de8:   e59f001c    ldr r0, [pc, #28]   ; e0c <register_io_dev_fip+0x38>
1337      dec:   eb00063b    bl  26e0 <__assert>
1338      df0:   e59f0018    ldr r0, [pc, #24]   ; e10 <register_io_dev_fip+0x3c>
1339      df4:   ebfffdd9    bl  560 <io_register_device>
1340      df8:   e3500000    cmp r0, #0
1341      dfc:   059f3010    ldreq   r3, [pc, #16]   ; e14 <register_io_dev_fip+0x40>
1342      e00:   05843000    streq   r3, [r4]
1343      e04:   e8bd8010    pop {r4, pc}
1344      e08:   00004035    .word   0x00004035
1345      e0c:   00004403    .word   0x00004403
1346      e10:   000042b8    .word   0x000042b8
1347      e14:   00004290    .word   0x00004290
1348 
1349 00000e18 <device_type_memmap>:
1350      e18:   e3a00002    mov r0, #2
1351      e1c:   e12fff1e    bx  lr
1352 
1353 00000e20 <memmap_dev_close>:
1354      e20:   e3a00000    mov r0, #0
1355      e24:   e12fff1e    bx  lr
1356 
1357 00000e28 <memmap_dev_open>:
1358      e28:   e3510000    cmp r1, #0
1359      e2c:   1a000004    bne e44 <memmap_dev_open+0x1c>
1360      e30:   e92d4010    push    {r4, lr}
1361      e34:   e3a01051    mov r1, #81 ; 0x51
1362      e38:   e59f2014    ldr r2, [pc, #20]   ; e54 <memmap_dev_open+0x2c>
1363      e3c:   e59f0014    ldr r0, [pc, #20]   ; e58 <memmap_dev_open+0x30>
1364      e40:   eb000626    bl  26e0 <__assert>
1365      e44:   e59f3010    ldr r3, [pc, #16]   ; e5c <memmap_dev_open+0x34>
1366      e48:   e3a00000    mov r0, #0
1367      e4c:   e5813000    str r3, [r1]
1368      e50:   e12fff1e    bx  lr
1369      e54:   00004078    .word   0x00004078
1370      e58:   00004574    .word   0x00004574
1371      e5c:   0000456c    .word   0x0000456c
1372 
1373 00000e60 <memmap_block_len>:
1374      e60:   e3500000    cmp r0, #0
1375      e64:   e92d4010    push    {r4, lr}
1376      e68:   059f2030    ldreq   r2, [pc, #48]   ; ea0 <memmap_block_len+0x40>
1377      e6c:   03a0109b    moveq   r1, #155    ; 0x9b
1378      e70:   0a000003    beq e84 <memmap_block_len+0x24>
1379      e74:   e3510000    cmp r1, #0
1380      e78:   1a000003    bne e8c <memmap_block_len+0x2c>
1381      e7c:   e59f2020    ldr r2, [pc, #32]   ; ea4 <memmap_block_len+0x44>
1382      e80:   e3a0109c    mov r1, #156    ; 0x9c
1383      e84:   e59f001c    ldr r0, [pc, #28]   ; ea8 <memmap_block_len+0x48>
1384      e88:   eb000614    bl  26e0 <__assert>
1385      e8c:   e5903004    ldr r3, [r0, #4]
1386      e90:   e3a00000    mov r0, #0
1387      e94:   e593300c    ldr r3, [r3, #12]
1388      e98:   e5813000    str r3, [r1]
1389      e9c:   e8bd8010    pop {r4, pc}
1390      ea0:   000040a5    .word   0x000040a5
1391      ea4:   000040b4    .word   0x000040b4
1392      ea8:   00004574    .word   0x00004574
1393 
1394 00000eac <memmap_block_seek>:
1395      eac:   e3510001    cmp r1, #1
1396      eb0:   1a000013    bne f04 <memmap_block_seek+0x58>
1397      eb4:   e3500000    cmp r0, #0
1398      eb8:   e92d4010    push    {r4, lr}
1399      ebc:   1a000003    bne ed0 <memmap_block_seek+0x24>
1400      ec0:   e59f2044    ldr r2, [pc, #68]   ; f0c <memmap_block_seek+0x60>
1401      ec4:   e3a01088    mov r1, #136    ; 0x88
1402      ec8:   e59f0040    ldr r0, [pc, #64]   ; f10 <memmap_block_seek+0x64>
1403      ecc:   eb000603    bl  26e0 <__assert>
1404      ed0:   e3520000    cmp r2, #0
1405      ed4:   e5903004    ldr r3, [r0, #4]
1406      ed8:   ba000002    blt ee8 <memmap_block_seek+0x3c>
1407      edc:   e593100c    ldr r1, [r3, #12]
1408      ee0:   e1510002    cmp r1, r2
1409      ee4:   8a000003    bhi ef8 <memmap_block_seek+0x4c>
1410      ee8:   e59f2024    ldr r2, [pc, #36]   ; f14 <memmap_block_seek+0x68>
1411      eec:   e3a0108d    mov r1, #141    ; 0x8d
1412      ef0:   e59f0018    ldr r0, [pc, #24]   ; f10 <memmap_block_seek+0x64>
1413      ef4:   eb0005f9    bl  26e0 <__assert>
1414      ef8:   e3a00000    mov r0, #0
1415      efc:   e5832008    str r2, [r3, #8]
1416      f00:   e8bd8010    pop {r4, pc}
1417      f04:   e3e00001    mvn r0, #1
1418      f08:   e12fff1e    bx  lr
1419      f0c:   000040a5    .word   0x000040a5
1420      f10:   00004574    .word   0x00004574
1421      f14:   000044e5    .word   0x000044e5
1422 
1423 00000f18 <memmap_block_close>:
1424      f18:   e3500000    cmp r0, #0
1425      f1c:   e92d4010    push    {r4, lr}
1426      f20:   1a000003    bne f34 <memmap_block_close+0x1c>
1427      f24:   e59f2024    ldr r2, [pc, #36]   ; f50 <memmap_block_close+0x38>
1428      f28:   e3a010df    mov r1, #223    ; 0xdf
1429      f2c:   e59f0020    ldr r0, [pc, #32]   ; f54 <memmap_block_close+0x3c>
1430      f30:   eb0005ea    bl  26e0 <__assert>
1431      f34:   e3a04000    mov r4, #0
1432      f38:   e3a01010    mov r1, #16
1433      f3c:   e5804004    str r4, [r0, #4]
1434      f40:   e59f0010    ldr r0, [pc, #16]   ; f58 <memmap_block_close+0x40>
1435      f44:   eb000b4d    bl  3c80 <zero_normalmem>
1436      f48:   e1a00004    mov r0, r4
1437      f4c:   e8bd8010    pop {r4, pc}
1438      f50:   000040a5    .word   0x000040a5
1439      f54:   00004574    .word   0x00004574
1440      f58:   0e02f130    .word   0x0e02f130
1441 
1442 00000f5c <memmap_block_write>:
1443      f5c:   e3500000    cmp r0, #0
1444      f60:   e92d41f0    push    {r4, r5, r6, r7, r8, lr}
1445      f64:   059f2078    ldreq   r2, [pc, #120]  ; fe4 <memmap_block_write+0x88>
1446      f68:   03a010c7    moveq   r1, #199    ; 0xc7
1447      f6c:   0a000003    beq f80 <memmap_block_write+0x24>
1448      f70:   e3510000    cmp r1, #0
1449      f74:   1a000003    bne f88 <memmap_block_write+0x2c>
1450      f78:   e59f2068    ldr r2, [pc, #104]  ; fe8 <memmap_block_write+0x8c>
1451      f7c:   e3a010c8    mov r1, #200    ; 0xc8
1452      f80:   e59f0064    ldr r0, [pc, #100]  ; fec <memmap_block_write+0x90>
1453      f84:   eb0005d5    bl  26e0 <__assert>
1454      f88:   e3530000    cmp r3, #0
1455      f8c:   e1a07003    mov r7, r3
1456      f90:   059f2058    ldreq   r2, [pc, #88]   ; ff0 <memmap_block_write+0x94>
1457      f94:   03a010c9    moveq   r1, #201    ; 0xc9
1458      f98:   0afffff8    beq f80 <memmap_block_write+0x24>
1459      f9c:   e5904004    ldr r4, [r0, #4]
1460      fa0:   e1a05002    mov r5, r2
1461      fa4:   e5940008    ldr r0, [r4, #8]
1462      fa8:   e0906002    adds    r6, r0, r2
1463      fac:   2a000002    bcs fbc <memmap_block_write+0x60>
1464      fb0:   e594300c    ldr r3, [r4, #12]
1465      fb4:   e1530006    cmp r3, r6
1466      fb8:   2a000002    bcs fc8 <memmap_block_write+0x6c>
1467      fbc:   e59f2030    ldr r2, [pc, #48]   ; ff4 <memmap_block_write+0x98>
1468      fc0:   e3a010cf    mov r1, #207    ; 0xcf
1469      fc4:   eaffffed    b   f80 <memmap_block_write+0x24>
1470      fc8:   e5943004    ldr r3, [r4, #4]
1471      fcc:   e0800003    add r0, r0, r3
1472      fd0:   eb0005d6    bl  2730 <memcpy>
1473      fd4:   e3a00000    mov r0, #0
1474      fd8:   e5875000    str r5, [r7]
1475      fdc:   e5846008    str r6, [r4, #8]
1476      fe0:   e8bd81f0    pop {r4, r5, r6, r7, r8, pc}
1477      fe4:   000040a5    .word   0x000040a5
1478      fe8:   000040e9    .word   0x000040e9
1479      fec:   00004574    .word   0x00004574
1480      ff0:   00004103    .word   0x00004103
1481      ff4:   0000450a    .word   0x0000450a
1482 
1483 00000ff8 <memmap_block_read>:
1484      ff8:   e3500000    cmp r0, #0
1485      ffc:   e92d41f0    push    {r4, r5, r6, r7, r8, lr}
1486     1000:   059f2080    ldreq   r2, [pc, #128]  ; 1088 <memmap_block_read+0x90>
1487     1004:   03a010ab    moveq   r1, #171    ; 0xab
1488     1008:   0a000004    beq 1020 <memmap_block_read+0x28>
1489     100c:   e3510000    cmp r1, #0
1490     1010:   e1a0c001    mov ip, r1
1491     1014:   1a000003    bne 1028 <memmap_block_read+0x30>
1492     1018:   e59f206c    ldr r2, [pc, #108]  ; 108c <memmap_block_read+0x94>
1493     101c:   e3a010ac    mov r1, #172    ; 0xac
1494     1020:   e59f0068    ldr r0, [pc, #104]  ; 1090 <memmap_block_read+0x98>
1495     1024:   eb0005ad    bl  26e0 <__assert>
1496     1028:   e3530000    cmp r3, #0
1497     102c:   e1a07003    mov r7, r3
1498     1030:   059f205c    ldreq   r2, [pc, #92]   ; 1094 <memmap_block_read+0x9c>
1499     1034:   03a010ad    moveq   r1, #173    ; 0xad
1500     1038:   0afffff8    beq 1020 <memmap_block_read+0x28>
1501     103c:   e5904004    ldr r4, [r0, #4]
1502     1040:   e1a05002    mov r5, r2
1503     1044:   e5941008    ldr r1, [r4, #8]
1504     1048:   e0916002    adds    r6, r1, r2
1505     104c:   2a000002    bcs 105c <memmap_block_read+0x64>
1506     1050:   e594300c    ldr r3, [r4, #12]
1507     1054:   e1530006    cmp r3, r6
1508     1058:   2a000002    bcs 1068 <memmap_block_read+0x70>
1509     105c:   e59f2034    ldr r2, [pc, #52]   ; 1098 <memmap_block_read+0xa0>
1510     1060:   e3a010b3    mov r1, #179    ; 0xb3
1511     1064:   eaffffed    b   1020 <memmap_block_read+0x28>
1512     1068:   e5943004    ldr r3, [r4, #4]
1513     106c:   e1a0000c    mov r0, ip
1514     1070:   e0811003    add r1, r1, r3
1515     1074:   eb0005ad    bl  2730 <memcpy>
1516     1078:   e3a00000    mov r0, #0
1517     107c:   e5875000    str r5, [r7]
1518     1080:   e5846008    str r6, [r4, #8]
1519     1084:   e8bd81f0    pop {r4, r5, r6, r7, r8, pc}
1520     1088:   000040a5    .word   0x000040a5
1521     108c:   000040e9    .word   0x000040e9
1522     1090:   00004574    .word   0x00004574
1523     1094:   000040d5    .word   0x000040d5
1524     1098:   0000450a    .word   0x0000450a
1525 
1526 0000109c <memmap_block_open>:
1527     109c:   e59f3064    ldr r3, [pc, #100]  ; 1108 <memmap_block_open+0x6c>
1528     10a0:   e92d4010    push    {r4, lr}
1529     10a4:   e5930000    ldr r0, [r3]
1530     10a8:   e3500000    cmp r0, #0
1531     10ac:   1a000011    bne 10f8 <memmap_block_open+0x5c>
1532     10b0:   e3510000    cmp r1, #0
1533     10b4:   059f2050    ldreq   r2, [pc, #80]   ; 110c <memmap_block_open+0x70>
1534     10b8:   03a0106e    moveq   r1, #110    ; 0x6e
1535     10bc:   0a000003    beq 10d0 <memmap_block_open+0x34>
1536     10c0:   e3520000    cmp r2, #0
1537     10c4:   1a000003    bne 10d8 <memmap_block_open+0x3c>
1538     10c8:   e59f2040    ldr r2, [pc, #64]   ; 1110 <memmap_block_open+0x74>
1539     10cc:   e3a0106f    mov r1, #111    ; 0x6f
1540     10d0:   e59f003c    ldr r0, [pc, #60]   ; 1114 <memmap_block_open+0x78>
1541     10d4:   eb000581    bl  26e0 <__assert>
1542     10d8:   e3a0c001    mov ip, #1
1543     10dc:   e583c000    str ip, [r3]
1544     10e0:   e591c000    ldr ip, [r1]
1545     10e4:   e5911004    ldr r1, [r1, #4]
1546     10e8:   e583c004    str ip, [r3, #4]
1547     10ec:   e1c300f8    strd    r0, [r3, #8]
1548     10f0:   e5823004    str r3, [r2, #4]
1549     10f4:   e8bd8010    pop {r4, pc}
1550     10f8:   e59f0018    ldr r0, [pc, #24]   ; 1118 <memmap_block_open+0x7c>
1551     10fc:   eb0003c0    bl  2004 <tf_log>
1552     1100:   e3e0000b    mvn r0, #11
1553     1104:   e8bd8010    pop {r4, pc}
1554     1108:   0e02f130    .word   0x0e02f130
1555     110c:   000044a0    .word   0x000044a0
1556     1110:   000040a5    .word   0x000040a5
1557     1114:   00004574    .word   0x00004574
1558     1118:   000044b3    .word   0x000044b3
1559 
1560 0000111c <register_io_dev_memmap>:
1561     111c:   e92d4010    push    {r4, lr}
1562     1120:   e2504000    subs    r4, r0, #0
1563     1124:   1a000003    bne 1138 <register_io_dev_memmap+0x1c>
1564     1128:   e59f2020    ldr r2, [pc, #32]   ; 1150 <register_io_dev_memmap+0x34>
1565     112c:   e3a010f0    mov r1, #240    ; 0xf0
1566     1130:   e59f001c    ldr r0, [pc, #28]   ; 1154 <register_io_dev_memmap+0x38>
1567     1134:   eb000569    bl  26e0 <__assert>
1568     1138:   e59f0018    ldr r0, [pc, #24]   ; 1158 <register_io_dev_memmap+0x3c>
1569     113c:   ebfffd07    bl  560 <io_register_device>
1570     1140:   e3500000    cmp r0, #0
1571     1144:   059f3010    ldreq   r3, [pc, #16]   ; 115c <register_io_dev_memmap+0x40>
1572     1148:   05843000    streq   r3, [r4]
1573     114c:   e8bd8010    pop {r4, pc}
1574     1150:   00004035    .word   0x00004035
1575     1154:   00004574    .word   0x00004574
1576     1158:   0000456c    .word   0x0000456c
1577     115c:   00004544    .word   0x00004544
1578 
1579 00001160 <semihosting_file_open>:
1580     1160:   e92d401f    push    {r0, r1, r2, r3, r4, lr}
1581     1164:   e1cd00f4    strd    r0, [sp, #4]
1582     1168:   eb00058f    bl  27ac <strlen>
1583     116c:   e28d1004    add r1, sp, #4
1584     1170:   e58d000c    str r0, [sp, #12]
1585     1174:   e3a00001    mov r0, #1
1586     1178:   eb00098e    bl  37b8 <semihosting_call>
1587     117c:   e28dd014    add sp, sp, #20
1588     1180:   e49df004    pop {pc}        ; (ldr pc, [sp], #4)
1589 
1590 00001184 <semihosting_file_seek>:
1591     1184:   e92d4007    push    {r0, r1, r2, lr}
1592     1188:   e1cd00f0    strd    r0, [sp]
1593     118c:   e1a0100d    mov r1, sp
1594     1190:   e3a0000a    mov r0, #10
1595     1194:   eb000987    bl  37b8 <semihosting_call>
1596     1198:   e3500000    cmp r0, #0
1597     119c:   0a000002    beq 11ac <semihosting_file_seek+0x28>
1598     11a0:   e3a01000    mov r1, #0
1599     11a4:   e3a00013    mov r0, #19
1600     11a8:   eb000982    bl  37b8 <semihosting_call>
1601     11ac:   e28dd00c    add sp, sp, #12
1602     11b0:   e49df004    pop {pc}        ; (ldr pc, [sp], #4)
1603 
1604 000011b4 <semihosting_file_read>:
1605     11b4:   e1a03002    mov r3, r2
1606     11b8:   e16f2f12    clz r2, r2
1607     11bc:   e1a022a2    lsr r2, r2, #5
1608     11c0:   e3510000    cmp r1, #0
1609     11c4:   e92d4030    push    {r4, r5, lr}
1610     11c8:   03a05001    moveq   r5, #1
1611     11cc:   11a05002    movne   r5, r2
1612     11d0:   e24dd014    sub sp, sp, #20
1613     11d4:   e3550000    cmp r5, #0
1614     11d8:   0a000002    beq 11e8 <semihosting_file_read+0x34>
1615     11dc:   e3e00015    mvn r0, #21
1616     11e0:   e28dd014    add sp, sp, #20
1617     11e4:   e8bd8030    pop {r4, r5, pc}
1618     11e8:   e1a04001    mov r4, r1
1619     11ec:   e98d0009    stmib   sp, {r0, r3}
1620     11f0:   e28d1004    add r1, sp, #4
1621     11f4:   e5943000    ldr r3, [r4]
1622     11f8:   e3a00006    mov r0, #6
1623     11fc:   e58d300c    str r3, [sp, #12]
1624     1200:   eb00096c    bl  37b8 <semihosting_call>
1625     1204:   e5943000    ldr r3, [r4]
1626     1208:   e1530000    cmp r3, r0
1627     120c:   0afffff2    beq 11dc <semihosting_file_read+0x28>
1628     1210:   80430000    subhi   r0, r3, r0
1629     1214:   85840000    strhi   r0, [r4]
1630     1218:   81a00005    movhi   r0, r5
1631     121c:   eaffffef    b   11e0 <semihosting_file_read+0x2c>
1632 
1633 00001220 <semihosting_file_write>:
1634     1220:   e3520000    cmp r2, #0
1635     1224:   13510000    cmpne   r1, #0
1636     1228:   e92d401f    push    {r0, r1, r2, r3, r4, lr}
1637     122c:   0a000008    beq 1254 <semihosting_file_write+0x34>
1638     1230:   e1a04001    mov r4, r1
1639     1234:   e28d1004    add r1, sp, #4
1640     1238:   e5943000    ldr r3, [r4]
1641     123c:   e98d000d    stmib   sp, {r0, r2, r3}
1642     1240:   e3a00005    mov r0, #5
1643     1244:   eb00095b    bl  37b8 <semihosting_call>
1644     1248:   e3500000    cmp r0, #0
1645     124c:   e5840000    str r0, [r4]
1646     1250:   0a000000    beq 1258 <semihosting_file_write+0x38>
1647     1254:   e3e00015    mvn r0, #21
1648     1258:   e28dd010    add sp, sp, #16
1649     125c:   e8bd8010    pop {r4, pc}
1650 
1651 00001260 <semihosting_file_close>:
1652     1260:   e92d4007    push    {r0, r1, r2, lr}
1653     1264:   e28d1004    add r1, sp, #4
1654     1268:   e58d0004    str r0, [sp, #4]
1655     126c:   e3a00002    mov r0, #2
1656     1270:   eb000950    bl  37b8 <semihosting_call>
1657     1274:   e28dd00c    add sp, sp, #12
1658     1278:   e49df004    pop {pc}        ; (ldr pc, [sp], #4)
1659 
1660 0000127c <semihosting_file_length>:
1661     127c:   e92d4007    push    {r0, r1, r2, lr}
1662     1280:   e28d1004    add r1, sp, #4
1663     1284:   e58d0004    str r0, [sp, #4]
1664     1288:   e3a0000c    mov r0, #12
1665     128c:   eb000949    bl  37b8 <semihosting_call>
1666     1290:   e28dd00c    add sp, sp, #12
1667     1294:   e49df004    pop {pc}        ; (ldr pc, [sp], #4)
1668 
1669 00001298 <open_fip>:
1670     1298:   e92d4073    push    {r0, r1, r4, r5, r6, lr}
1671     129c:   e1a06000    mov r6, r0
1672     12a0:   e59f5044    ldr r5, [pc, #68]   ; 12ec <open_fip+0x54>
1673     12a4:   e3a01000    mov r1, #0
1674     12a8:   e5950000    ldr r0, [r5]
1675     12ac:   ebfffcd2    bl  5fc <io_dev_init>
1676     12b0:   e2504000    subs    r4, r0, #0
1677     12b4:   1a000009    bne 12e0 <open_fip+0x48>
1678     12b8:   e28d2004    add r2, sp, #4
1679     12bc:   e1a01006    mov r1, r6
1680     12c0:   e5950000    ldr r0, [r5]
1681     12c4:   ebfffcfe    bl  6c4 <io_open>
1682     12c8:   e2504000    subs    r4, r0, #0
1683     12cc:   1a000003    bne 12e0 <open_fip+0x48>
1684     12d0:   e59f0018    ldr r0, [pc, #24]   ; 12f0 <open_fip+0x58>
1685     12d4:   eb00034a    bl  2004 <tf_log>
1686     12d8:   e59d0004    ldr r0, [sp, #4]
1687     12dc:   ebfffd8e    bl  91c <io_close>
1688     12e0:   e1a00004    mov r0, r4
1689     12e4:   e28dd008    add sp, sp, #8
1690     12e8:   e8bd8070    pop {r4, r5, r6, pc}
1691     12ec:   0e02f144    .word   0x0e02f144
1692     12f0:   000045f4    .word   0x000045f4
1693 
1694 000012f4 <open_memmap>:
1695     12f4:   e92d4073    push    {r0, r1, r4, r5, r6, lr}
1696     12f8:   e1a06000    mov r6, r0
1697     12fc:   e59f5044    ldr r5, [pc, #68]   ; 1348 <open_memmap+0x54>
1698     1300:   e3a01000    mov r1, #0
1699     1304:   e5950000    ldr r0, [r5]
1700     1308:   ebfffcbb    bl  5fc <io_dev_init>
1701     130c:   e2504000    subs    r4, r0, #0
1702     1310:   1a000009    bne 133c <open_memmap+0x48>
1703     1314:   e28d2004    add r2, sp, #4
1704     1318:   e1a01006    mov r1, r6
1705     131c:   e5950000    ldr r0, [r5]
1706     1320:   ebfffce7    bl  6c4 <io_open>
1707     1324:   e2504000    subs    r4, r0, #0
1708     1328:   1a000003    bne 133c <open_memmap+0x48>
1709     132c:   e59f0018    ldr r0, [pc, #24]   ; 134c <open_memmap+0x58>
1710     1330:   eb000333    bl  2004 <tf_log>
1711     1334:   e59d0004    ldr r0, [sp, #4]
1712     1338:   ebfffd77    bl  91c <io_close>
1713     133c:   e1a00004    mov r0, r4
1714     1340:   e28dd008    add sp, sp, #8
1715     1344:   e8bd8070    pop {r4, r5, r6, pc}
1716     1348:   0e02f14c    .word   0x0e02f14c
1717     134c:   00004600    .word   0x00004600
1718 
1719 00001350 <plat_qemu_io_setup>:
1720     1350:   e92d4070    push    {r4, r5, r6, lr}
1721     1354:   e59f50ac    ldr r5, [pc, #172]  ; 1408 <plat_qemu_io_setup+0xb8>
1722     1358:   e1a00005    mov r0, r5
1723     135c:   ebfffe9c    bl  dd4 <register_io_dev_fip>
1724     1360:   e3500000    cmp r0, #0
1725     1364:   159f20a0    ldrne   r2, [pc, #160]  ; 140c <plat_qemu_io_setup+0xbc>
1726     1368:   13a01d05    movne   r1, #320    ; 0x140
1727     136c:   1a000006    bne 138c <plat_qemu_io_setup+0x3c>
1728     1370:   e59f4098    ldr r4, [pc, #152]  ; 1410 <plat_qemu_io_setup+0xc0>
1729     1374:   e1a00004    mov r0, r4
1730     1378:   ebffff67    bl  111c <register_io_dev_memmap>
1731     137c:   e2501000    subs    r1, r0, #0
1732     1380:   0a000003    beq 1394 <plat_qemu_io_setup+0x44>
1733     1384:   e59f2080    ldr r2, [pc, #128]  ; 140c <plat_qemu_io_setup+0xbc>
1734     1388:   e3001143    movw    r1, #323    ; 0x143
1735     138c:   e59f0080    ldr r0, [pc, #128]  ; 1414 <plat_qemu_io_setup+0xc4>
1736     1390:   eb0004d2    bl  26e0 <__assert>
1737     1394:   e59f207c    ldr r2, [pc, #124]  ; 1418 <plat_qemu_io_setup+0xc8>
1738     1398:   e5950000    ldr r0, [r5]
1739     139c:   ebfffc81    bl  5a8 <io_dev_open>
1740     13a0:   e2501000    subs    r1, r0, #0
1741     13a4:   159f2060    ldrne   r2, [pc, #96]   ; 140c <plat_qemu_io_setup+0xbc>
1742     13a8:   13a01f52    movne   r1, #328    ; 0x148
1743     13ac:   1afffff6    bne 138c <plat_qemu_io_setup+0x3c>
1744     13b0:   e59f2064    ldr r2, [pc, #100]  ; 141c <plat_qemu_io_setup+0xcc>
1745     13b4:   e5940000    ldr r0, [r4]
1746     13b8:   ebfffc7a    bl  5a8 <io_dev_open>
1747     13bc:   e3500000    cmp r0, #0
1748     13c0:   159f2044    ldrne   r2, [pc, #68]   ; 140c <plat_qemu_io_setup+0xbc>
1749     13c4:   13a01f53    movne   r1, #332    ; 0x14c
1750     13c8:   1affffef    bne 138c <plat_qemu_io_setup+0x3c>
1751     13cc:   e59f404c    ldr r4, [pc, #76]   ; 1420 <plat_qemu_io_setup+0xd0>
1752     13d0:   e1a00004    mov r0, r4
1753     13d4:   ebfffc1d    bl  450 <register_io_dev_sh>
1754     13d8:   e2501000    subs    r1, r0, #0
1755     13dc:   159f2028    ldrne   r2, [pc, #40]   ; 140c <plat_qemu_io_setup+0xbc>
1756     13e0:   13a01e15    movne   r1, #336    ; 0x150
1757     13e4:   1affffe8    bne 138c <plat_qemu_io_setup+0x3c>
1758     13e8:   e59f2034    ldr r2, [pc, #52]   ; 1424 <plat_qemu_io_setup+0xd4>
1759     13ec:   e5940000    ldr r0, [r4]
1760     13f0:   ebfffc6c    bl  5a8 <io_dev_open>
1761     13f4:   e3500000    cmp r0, #0
1762     13f8:   159f200c    ldrne   r2, [pc, #12]   ; 140c <plat_qemu_io_setup+0xbc>
1763     13fc:   13a01f55    movne   r1, #340    ; 0x154
1764     1400:   08bd8070    popeq   {r4, r5, r6, pc}
1765     1404:   eaffffe0    b   138c <plat_qemu_io_setup+0x3c>
1766     1408:   0e02f140    .word   0x0e02f140
1767     140c:   0000465f    .word   0x0000465f
1768     1410:   0e02f148    .word   0x0e02f148
1769     1414:   0000466e    .word   0x0000466e
1770     1418:   0e02f144    .word   0x0e02f144
1771     141c:   0e02f14c    .word   0x0e02f14c
1772     1420:   0e02f150    .word   0x0e02f150
1773     1424:   0e02f154    .word   0x0e02f154
1774 
1775 00001428 <plat_get_image_source>:
1776     1428:   e3500016    cmp r0, #22
1777     142c:   e92d47f3    push    {r0, r1, r4, r5, r6, r7, r8, r9, sl, lr}
1778     1430:   9a000003    bls 1444 <plat_get_image_source+0x1c>
1779     1434:   e59f20b8    ldr r2, [pc, #184]  ; 14f4 <plat_get_image_source+0xcc>
1780     1438:   e3001171    movw    r1, #369    ; 0x171
1781     143c:   e59f00b4    ldr r0, [pc, #180]  ; 14f8 <plat_get_image_source+0xd0>
1782     1440:   eb0004a6    bl  26e0 <__assert>
1783     1444:   e3a0500c    mov r5, #12
1784     1448:   e59f40ac    ldr r4, [pc, #172]  ; 14fc <plat_get_image_source+0xd4>
1785     144c:   e1a09000    mov r9, r0
1786     1450:   e1a08001    mov r8, r1
1787     1454:   e1a07002    mov r7, r2
1788     1458:   e0050095    mul r5, r5, r0
1789     145c:   e0843005    add r3, r4, r5
1790     1460:   e593a004    ldr sl, [r3, #4]
1791     1464:   e5933008    ldr r3, [r3, #8]
1792     1468:   e1a0000a    mov r0, sl
1793     146c:   e12fff33    blx r3
1794     1470:   e2506000    subs    r6, r0, #0
1795     1474:   1a000006    bne 1494 <plat_get_image_source+0x6c>
1796     1478:   e7943005    ldr r3, [r4, r5]
1797     147c:   e587a000    str sl, [r7]
1798     1480:   e5933000    ldr r3, [r3]
1799     1484:   e5883000    str r3, [r8]
1800     1488:   e1a00006    mov r0, r6
1801     148c:   e28dd008    add sp, sp, #8
1802     1490:   e8bd87f0    pop {r4, r5, r6, r7, r8, r9, sl, pc}
1803     1494:   e59f5064    ldr r5, [pc, #100]  ; 1500 <plat_get_image_source+0xd8>
1804     1498:   e59f0064    ldr r0, [pc, #100]  ; 1504 <plat_get_image_source+0xdc>
1805     149c:   eb0002d8    bl  2004 <tf_log>
1806     14a0:   e3a01000    mov r1, #0
1807     14a4:   e5950000    ldr r0, [r5]
1808     14a8:   ebfffc53    bl  5fc <io_dev_init>
1809     14ac:   e2506000    subs    r6, r0, #0
1810     14b0:   1afffff4    bne 1488 <plat_get_image_source+0x60>
1811     14b4:   e59f404c    ldr r4, [pc, #76]   ; 1508 <plat_get_image_source+0xe0>
1812     14b8:   e28d2004    add r2, sp, #4
1813     14bc:   e5950000    ldr r0, [r5]
1814     14c0:   e0844189    add r4, r4, r9, lsl #3
1815     14c4:   e1a01004    mov r1, r4
1816     14c8:   ebfffc7d    bl  6c4 <io_open>
1817     14cc:   e2506000    subs    r6, r0, #0
1818     14d0:   1affffec    bne 1488 <plat_get_image_source+0x60>
1819     14d4:   e59f0030    ldr r0, [pc, #48]   ; 150c <plat_get_image_source+0xe4>
1820     14d8:   eb0002c9    bl  2004 <tf_log>
1821     14dc:   e59d0004    ldr r0, [sp, #4]
1822     14e0:   ebfffd0d    bl  91c <io_close>
1823     14e4:   e5953000    ldr r3, [r5]
1824     14e8:   e5883000    str r3, [r8]
1825     14ec:   e5874000    str r4, [r7]
1826     14f0:   eaffffe4    b   1488 <plat_get_image_source+0x60>
1827     14f4:   0000460f    .word   0x0000460f
1828     14f8:   0000466e    .word   0x0000466e
1829     14fc:   0000468c    .word   0x0000468c
1830     1500:   0e02f154    .word   0x0e02f154
1831     1504:   0000462f    .word   0x0000462f
1832     1508:   000047a0    .word   0x000047a0
1833     150c:   00004647    .word   0x00004647
1834 
1835 00001510 <bl1_plat_sec_mem_layout>:
1836     1510:   e59f0000    ldr r0, [pc]    ; 1518 <bl1_plat_sec_mem_layout+0x8>
1837     1514:   e12fff1e    bx  lr
1838     1518:   0e02f158    .word   0x0e02f158
1839 
1840 0000151c <bl1_early_platform_setup>:
1841     151c:   e3a01001    mov r1, #1
1842     1520:   e92d4010    push    {r4, lr}
1843     1524:   e59f2018    ldr r2, [pc, #24]   ; 1544 <bl1_early_platform_setup+0x28>
1844     1528:   e3a00409    mov r0, #150994944  ; 0x9000000
1845     152c:   eb000a19    bl  3d98 <console_init>
1846     1530:   e59f3010    ldr r3, [pc, #16]   ; 1548 <bl1_early_platform_setup+0x2c>
1847     1534:   e3a01a3f    mov r1, #258048 ; 0x3f000
1848     1538:   e59f000c    ldr r0, [pc, #12]   ; 154c <bl1_early_platform_setup+0x30>
1849     153c:   e1c300f0    strd    r0, [r3]
1850     1540:   e8bd8010    pop {r4, pc}
1851     1544:   0001c200    .word   0x0001c200
1852     1548:   0e02f158    .word   0x0e02f158
1853     154c:   0e001000    .word   0x0e001000
1854 
1855 00001550 <bl1_plat_arch_setup>:
1856     1550:   e92d401f    push    {r0, r1, r2, r3, r4, lr}
1857     1554:   e59f3040    ldr r3, [pc, #64]   ; 159c <bl1_plat_arch_setup+0x4c>
1858     1558:   e59f0040    ldr r0, [pc, #64]   ; 15a0 <bl1_plat_arch_setup+0x50>
1859     155c:   e59f2040    ldr r2, [pc, #64]   ; 15a4 <bl1_plat_arch_setup+0x54>
1860     1560:   e58d300c    str r3, [sp, #12]
1861     1564:   e59f303c    ldr r3, [pc, #60]   ; 15a8 <bl1_plat_arch_setup+0x58>
1862     1568:   e58d3008    str r3, [sp, #8]
1863     156c:   e59f3038    ldr r3, [pc, #56]   ; 15ac <bl1_plat_arch_setup+0x5c>
1864     1570:   e1e03623    mvn r3, r3, lsr #12
1865     1574:   e1e03603    mvn r3, r3, lsl #12
1866     1578:   e2833001    add r3, r3, #1
1867     157c:   e58d3004    str r3, [sp, #4]
1868     1580:   e59f3028    ldr r3, [pc, #40]   ; 15b0 <bl1_plat_arch_setup+0x60>
1869     1584:   e58d3000    str r3, [sp]
1870     1588:   e59f3024    ldr r3, [pc, #36]   ; 15b4 <bl1_plat_arch_setup+0x64>
1871     158c:   e1c000d0    ldrd    r0, [r0]
1872     1590:   eb00048d    bl  27cc <qemu_configure_mmu_secure>
1873     1594:   e28dd014    add sp, sp, #20
1874     1598:   e49df004    pop {pc}        ; (ldr pc, [sp], #4)
1875     159c:   0e036000    .word   0x0e036000
1876     15a0:   0e02f158    .word   0x0e02f158
1877     15a4:   00000000    .word   0x00000000
1878     15a8:   0e036000    .word   0x0e036000
1879     15ac:   0000588b    .word   0x0000588b
1880     15b0:   00004000    .word   0x00004000
1881     15b4:   00004000    .word   0x00004000
1882 
1883 000015b8 <bl1_platform_setup>:
1884     15b8:   eaffff64    b   1350 <plat_qemu_io_setup>
1885 
1886 000015bc <bl1_calc_bl2_mem_layout>:
1887     15bc:   e3500000    cmp r0, #0
1888     15c0:   e92d4010    push    {r4, lr}
1889     15c4:   059f2058    ldreq   r2, [pc, #88]   ; 1624 <bl1_calc_bl2_mem_layout+0x68>
1890     15c8:   03a01025    moveq   r1, #37 ; 0x25
1891     15cc:   0a000004    beq 15e4 <bl1_calc_bl2_mem_layout+0x28>
1892     15d0:   e3510000    cmp r1, #0
1893     15d4:   e1a02001    mov r2, r1
1894     15d8:   1a000003    bne 15ec <bl1_calc_bl2_mem_layout+0x30>
1895     15dc:   e59f2044    ldr r2, [pc, #68]   ; 1628 <bl1_calc_bl2_mem_layout+0x6c>
1896     15e0:   e3a01026    mov r1, #38 ; 0x26
1897     15e4:   e59f0040    ldr r0, [pc, #64]   ; 162c <bl1_calc_bl2_mem_layout+0x70>
1898     15e8:   eb00043c    bl  26e0 <__assert>
1899     15ec:   e59f103c    ldr r1, [pc, #60]   ; 1630 <bl1_calc_bl2_mem_layout+0x74>
1900     15f0:   e5903000    ldr r3, [r0]
1901     15f4:   e1530001    cmp r3, r1
1902     15f8:   859f2034    ldrhi   r2, [pc, #52]   ; 1634 <bl1_calc_bl2_mem_layout+0x78>
1903     15fc:   83a0102d    movhi   r1, #45 ; 0x2d
1904     1600:   8afffff7    bhi 15e4 <bl1_calc_bl2_mem_layout+0x28>
1905     1604:   e8bd4010    pop {r4, lr}
1906     1608:   e3a01008    mov r1, #8
1907     160c:   e5823000    str r3, [r2]
1908     1610:   e263340e    rsb r3, r3, #234881024  ; 0xe000000
1909     1614:   e1a00002    mov r0, r2
1910     1618:   e2833a2e    add r3, r3, #188416 ; 0x2e000
1911     161c:   e5823004    str r3, [r2, #4]
1912     1620:   ea000987    b   3c44 <flush_dcache_range>
1913     1624:   0000489b    .word   0x0000489b
1914     1628:   000048c1    .word   0x000048c1
1915     162c:   000048b2    .word   0x000048b2
1916     1630:   0e02dfff    .word   0x0e02dfff
1917     1634:   000048d8    .word   0x000048d8
1918 
1919 00001638 <bl1_init_bl2_mem_layout>:
1920     1638:   eaffffdf    b   15bc <bl1_calc_bl2_mem_layout>
1921 
1922 0000163c <bl1_main>:
1923     163c:   e92d4070    push    {r4, r5, r6, lr}
1924     1640:   e59f013c    ldr r0, [pc, #316]  ; 1784 <bl1_main+0x148>
1925     1644:   eb00026e    bl  2004 <tf_log>
1926     1648:   e59f1138    ldr r1, [pc, #312]  ; 1788 <bl1_main+0x14c>
1927     164c:   e59f0138    ldr r0, [pc, #312]  ; 178c <bl1_main+0x150>
1928     1650:   eb00026b    bl  2004 <tf_log>
1929     1654:   e59f1134    ldr r1, [pc, #308]  ; 1790 <bl1_main+0x154>
1930     1658:   e59f012c    ldr r0, [pc, #300]  ; 178c <bl1_main+0x150>
1931     165c:   eb000268    bl  2004 <tf_log>
1932     1660:   e59f212c    ldr r2, [pc, #300]  ; 1794 <bl1_main+0x158>
1933     1664:   e59f112c    ldr r1, [pc, #300]  ; 1798 <bl1_main+0x15c>
1934     1668:   e59f012c    ldr r0, [pc, #300]  ; 179c <bl1_main+0x160>
1935     166c:   eb000264    bl  2004 <tf_log>
1936     1670:   eb00092b    bl  3b24 <print_errata_status>
1937     1674:   ee113f10    mrc 15, 0, r3, cr1, cr0, {0}
1938     1678:   e3130001    tst r3, #1
1939     167c:   059f211c    ldreq   r2, [pc, #284]  ; 17a0 <bl1_main+0x164>
1940     1680:   03a01071    moveq   r1, #113    ; 0x71
1941     1684:   0a000003    beq 1698 <bl1_main+0x5c>
1942     1688:   e3130004    tst r3, #4
1943     168c:   1a000003    bne 16a0 <bl1_main+0x64>
1944     1690:   e59f210c    ldr r2, [pc, #268]  ; 17a4 <bl1_main+0x168>
1945     1694:   e3a01072    mov r1, #114    ; 0x72
1946     1698:   e59f0108    ldr r0, [pc, #264]  ; 17a8 <bl1_main+0x16c>
1947     169c:   eb00040f    bl  26e0 <__assert>
1948     16a0:   e3130a01    tst r3, #4096   ; 0x1000
1949     16a4:   059f2100    ldreq   r2, [pc, #256]  ; 17ac <bl1_main+0x170>
1950     16a8:   03a01073    moveq   r1, #115    ; 0x73
1951     16ac:   0afffff9    beq 1698 <bl1_main+0x5c>
1952     16b0:   ee103f30    mrc 15, 0, r3, cr0, cr0, {1}
1953     16b4:   e1a03c23    lsr r3, r3, #24
1954     16b8:   e313000b    tst r3, #11
1955     16bc:   159f20ec    ldrne   r2, [pc, #236]  ; 17b0 <bl1_main+0x174>
1956     16c0:   13a0107f    movne   r1, #127    ; 0x7f
1957     16c4:   1afffff3    bne 1698 <bl1_main+0x5c>
1958     16c8:   eb000076    bl  18a8 <bl1_arch_setup>
1959     16cc:   ebffffb9    bl  15b8 <bl1_platform_setup>
1960     16d0:   eb00019d    bl  1d4c <bl1_plat_get_next_image_id>
1961     16d4:   e3500001    cmp r0, #1
1962     16d8:   e1a04000    mov r4, r0
1963     16dc:   159f00d0    ldrne   r0, [pc, #208]  ; 17b4 <bl1_main+0x178>
1964     16e0:   1a000022    bne 1770 <bl1_main+0x134>
1965     16e4:   eb00019d    bl  1d60 <bl1_plat_get_image_desc>
1966     16e8:   e3500000    cmp r0, #0
1967     16ec:   059f20c4    ldreq   r2, [pc, #196]  ; 17b8 <bl1_main+0x17c>
1968     16f0:   03a010ae    moveq   r1, #174    ; 0xae
1969     16f4:   0affffe7    beq 1698 <bl1_main+0x5c>
1970     16f8:   e280500c    add r5, r0, #12
1971     16fc:   e59f00b8    ldr r0, [pc, #184]  ; 17bc <bl1_main+0x180>
1972     1700:   eb00023f    bl  2004 <tf_log>
1973     1704:   e1a00004    mov r0, r4
1974     1708:   eb000192    bl  1d58 <bl1_plat_handle_pre_image_load>
1975     170c:   e2506000    subs    r6, r0, #0
1976     1710:   0a000004    beq 1728 <bl1_main+0xec>
1977     1714:   e59f00a4    ldr r0, [pc, #164]  ; 17c0 <bl1_main+0x184>
1978     1718:   e1a01006    mov r1, r6
1979     171c:   eb000238    bl  2004 <tf_log>
1980     1720:   e1a00006    mov r0, r6
1981     1724:   eb000319    bl  2390 <plat_error_handler>
1982     1728:   e1a01005    mov r1, r5
1983     172c:   e1a00004    mov r0, r4
1984     1730:   eb0001a6    bl  1dd0 <load_auth_image>
1985     1734:   e2505000    subs    r5, r0, #0
1986     1738:   0a000003    beq 174c <bl1_main+0x110>
1987     173c:   e59f0080    ldr r0, [pc, #128]  ; 17c4 <bl1_main+0x188>
1988     1740:   eb00022f    bl  2004 <tf_log>
1989     1744:   e1a00005    mov r0, r5
1990     1748:   eafffff5    b   1724 <bl1_main+0xe8>
1991     174c:   e1a00004    mov r0, r4
1992     1750:   eb000185    bl  1d6c <bl1_plat_handle_post_image_load>
1993     1754:   e2505000    subs    r5, r0, #0
1994     1758:   059f0068    ldreq   r0, [pc, #104]  ; 17c8 <bl1_main+0x18c>
1995     175c:   0a000003    beq 1770 <bl1_main+0x134>
1996     1760:   e1a01005    mov r1, r5
1997     1764:   e59f0060    ldr r0, [pc, #96]   ; 17cc <bl1_main+0x190>
1998     1768:   eb000225    bl  2004 <tf_log>
1999     176c:   eafffff4    b   1744 <bl1_main+0x108>
2000     1770:   eb000223    bl  2004 <tf_log>
2001     1774:   e1a00004    mov r0, r4
2002     1778:   eb00007a    bl  1968 <bl1_prepare_next_image>
2003     177c:   e8bd4070    pop {r4, r5, r6, lr}
2004     1780:   ea00098f    b   3dc4 <console_flush>
2005     1784:   00004901    .word   0x00004901
2006     1788:   0000401e    .word   0x0000401e
2007     178c:   0000491c    .word   0x0000491c
2008     1790:   00004000    .word   0x00004000
2009     1794:   0e036000    .word   0x0e036000
2010     1798:   0e02e000    .word   0x0e02e000
2011     179c:   00004926    .word   0x00004926
2012     17a0:   00004939    .word   0x00004939
2013     17a4:   0000494b    .word   0x0000494b
2014     17a8:   000048b2    .word   0x000048b2
2015     17ac:   0000495d    .word   0x0000495d
2016     17b0:   0000496f    .word   0x0000496f
2017     17b4:   00004a57    .word   0x00004a57
2018     17b8:   000049a4    .word   0x000049a4
2019     17bc:   000049af    .word   0x000049af
2020     17c0:   000049c2    .word   0x000049c2
2021     17c4:   000049f3    .word   0x000049f3
2022     17c8:   00004a44    .word   0x00004a44
2023     17cc:   00004a12    .word   0x00004a12
2024 
2025 000017d0 <bl1_print_next_bl_ep_info>:
2026     17d0:   e92d4010    push    {r4, lr}
2027     17d4:   e1a04000    mov r4, r0
2028     17d8:   e59f000c    ldr r0, [pc, #12]   ; 17ec <bl1_print_next_bl_ep_info+0x1c>
2029     17dc:   eb000208    bl  2004 <tf_log>
2030     17e0:   e1a00004    mov r0, r4
2031     17e4:   e8bd4010    pop {r4, lr}
2032     17e8:   ea0001e5    b   1f84 <print_entry_point_info>
2033     17ec:   00004a84    .word   0x00004a84
2034 
2035 000017f0 <bl1_smc_handler>:
2036     17f0:   e3500001    cmp r0, #1
2037     17f4:   e92d4010    push    {r4, lr}
2038     17f8:   e59d4010    ldr r4, [sp, #16]
2039     17fc:   0a00000a    beq 182c <bl1_smc_handler+0x3c>
2040     1800:   e3500000    cmp r0, #0
2041     1804:   03a03004    moveq   r3, #4
2042     1808:   0a00000f    beq 184c <bl1_smc_handler+0x5c>
2043     180c:   e3500003    cmp r0, #3
2044     1810:   03a03001    moveq   r3, #1
2045     1814:   0a00000c    beq 184c <bl1_smc_handler+0x5c>
2046     1818:   e1a01000    mov r1, r0
2047     181c:   e59f0034    ldr r0, [pc, #52]   ; 1858 <bl1_smc_handler+0x68>
2048     1820:   eb0001f7    bl  2004 <tf_log>
2049     1824:   e3e03000    mvn r3, #0
2050     1828:   ea000007    b   184c <bl1_smc_handler+0x5c>
2051     182c:   e59f3028    ldr r3, [pc, #40]   ; 185c <bl1_smc_handler+0x6c>
2052     1830:   e593200c    ldr r2, [r3, #12]
2053     1834:   e584200c    str r2, [r4, #12]
2054     1838:   e5932008    ldr r2, [r3, #8]
2055     183c:   e5933004    ldr r3, [r3, #4]
2056     1840:   e5843004    str r3, [r4, #4]
2057     1844:   e59f3014    ldr r3, [pc, #20]   ; 1860 <bl1_smc_handler+0x70>
2058     1848:   e5842008    str r2, [r4, #8]
2059     184c:   e1a00004    mov r0, r4
2060     1850:   e5843000    str r3, [r4]
2061     1854:   e8bd8010    pop {r4, pc}
2062     1858:   00004a98    .word   0x00004a98
2063     185c:   00004ac4    .word   0x00004ac4
2064     1860:   fd3967d4    .word   0xfd3967d4
2065 
2066 00001864 <bl1_smc_wrapper>:
2067     1864:   e252c000    subs    ip, r2, #0
2068     1868:   e92d401f    push    {r0, r1, r2, r3, r4, lr}
2069     186c:   1a000003    bne 1880 <bl1_smc_wrapper+0x1c>
2070     1870:   e59f2028    ldr r2, [pc, #40]   ; 18a0 <bl1_smc_wrapper+0x3c>
2071     1874:   e300112a    movw    r1, #298    ; 0x12a
2072     1878:   e59f0024    ldr r0, [pc, #36]   ; 18a4 <bl1_smc_wrapper+0x40>
2073     187c:   eb000397    bl  26e0 <__assert>
2074     1880:   e98d1002    stmib   sp, {r1, ip}
2075     1884:   e58d300c    str r3, [sp, #12]
2076     1888:   e59c3010    ldr r3, [ip, #16]
2077     188c:   e58d3000    str r3, [sp]
2078     1890:   e99c000e    ldmib   ip, {r1, r2, r3}
2079     1894:   ebffffd5    bl  17f0 <bl1_smc_handler>
2080     1898:   e28dd014    add sp, sp, #20
2081     189c:   e49df004    pop {pc}        ; (ldr pc, [sp], #4)
2082     18a0:   00004abc    .word   0x00004abc
2083     18a4:   000048b2    .word   0x000048b2
2084 
2085 000018a8 <bl1_arch_setup>:
2086     18a8:   e12fff1e    bx  lr
2087 
2088 000018ac <smc_set_next_ctx>:
2089     18ac:   e3500001    cmp r0, #1
2090     18b0:   9a000004    bls 18c8 <smc_set_next_ctx+0x1c>
2091     18b4:   e92d4010    push    {r4, lr}
2092     18b8:   e3a01026    mov r1, #38 ; 0x26
2093     18bc:   e59f201c    ldr r2, [pc, #28]   ; 18e0 <smc_set_next_ctx+0x34>
2094     18c0:   e59f001c    ldr r0, [pc, #28]   ; 18e4 <smc_set_next_ctx+0x38>
2095     18c4:   eb000385    bl  26e0 <__assert>
2096     18c8:   e59f3018    ldr r3, [pc, #24]   ; 18e8 <smc_set_next_ctx+0x3c>
2097     18cc:   e3a02090    mov r2, #144    ; 0x90
2098     18d0:   e0203092    mla r0, r2, r0, r3
2099     18d4:   e59f3010    ldr r3, [pc, #16]   ; 18ec <smc_set_next_ctx+0x40>
2100     18d8:   e5830000    str r0, [r3]
2101     18dc:   e12fff1e    bx  lr
2102     18e0:   00004ae0    .word   0x00004ae0
2103     18e4:   00004b03    .word   0x00004b03
2104     18e8:   0e02f1a8    .word   0x0e02f1a8
2105     18ec:   0e02f1a4    .word   0x0e02f1a4
2106 
2107 000018f0 <smc_get_next_ctx>:
2108     18f0:   e59f3004    ldr r3, [pc, #4]    ; 18fc <smc_get_next_ctx+0xc>
2109     18f4:   e5930000    ldr r0, [r3]
2110     18f8:   e12fff1e    bx  lr
2111     18fc:   0e02f1a4    .word   0x0e02f1a4
2112 
2113 00001900 <cm_get_context>:
2114     1900:   e3500001    cmp r0, #1
2115     1904:   9a000004    bls 191c <cm_get_context+0x1c>
2116     1908:   e92d4010    push    {r4, lr}
2117     190c:   e3a01032    mov r1, #50 ; 0x32
2118     1910:   e59f2010    ldr r2, [pc, #16]   ; 1928 <cm_get_context+0x28>
2119     1914:   e59f0010    ldr r0, [pc, #16]   ; 192c <cm_get_context+0x2c>
2120     1918:   eb000370    bl  26e0 <__assert>
2121     191c:   e59f300c    ldr r3, [pc, #12]   ; 1930 <cm_get_context+0x30>
2122     1920:   e0830280    add r0, r3, r0, lsl #5
2123     1924:   e12fff1e    bx  lr
2124     1928:   00004ae0    .word   0x00004ae0
2125     192c:   00004b03    .word   0x00004b03
2126     1930:   0e02f160    .word   0x0e02f160
2127 
2128 00001934 <cm_set_next_context>:
2129     1934:   e3500000    cmp r0, #0
2130     1938:   1a000004    bne 1950 <cm_set_next_context+0x1c>
2131     193c:   e92d4010    push    {r4, lr}
2132     1940:   e3a01038    mov r1, #56 ; 0x38
2133     1944:   e59f2010    ldr r2, [pc, #16]   ; 195c <cm_set_next_context+0x28>
2134     1948:   e59f0010    ldr r0, [pc, #16]   ; 1960 <cm_set_next_context+0x2c>
2135     194c:   eb000363    bl  26e0 <__assert>
2136     1950:   e59f300c    ldr r3, [pc, #12]   ; 1964 <cm_set_next_context+0x30>
2137     1954:   e5830000    str r0, [r3]
2138     1958:   e12fff1e    bx  lr
2139     195c:   00004ad4    .word   0x00004ad4
2140     1960:   00004b03    .word   0x00004b03
2141     1964:   0e02f1a0    .word   0x0e02f1a0
2142 
2143 00001968 <bl1_prepare_next_image>:
2144     1968:   e92d41f0    push    {r4, r5, r6, r7, r8, lr}
2145     196c:   e1a07000    mov r7, r0
2146     1970:   eb0000fa    bl  1d60 <bl1_plat_get_image_desc>
2147     1974:   e2505000    subs    r5, r0, #0
2148     1978:   1a000003    bne 198c <bl1_prepare_next_image+0x24>
2149     197c:   e59f212c    ldr r2, [pc, #300]  ; 1ab0 <bl1_prepare_next_image+0x148>
2150     1980:   e3a0106d    mov r1, #109    ; 0x6d
2151     1984:   e59f0128    ldr r0, [pc, #296]  ; 1ab4 <bl1_prepare_next_image+0x14c>
2152     1988:   eb000354    bl  26e0 <__assert>
2153     198c:   e5954024    ldr r4, [r5, #36]   ; 0x24
2154     1990:   e2856020    add r6, r5, #32
2155     1994:   e2144001    ands    r4, r4, #1
2156     1998:   1a000001    bne 19a4 <bl1_prepare_next_image+0x3c>
2157     199c:   e30031d3    movw    r3, #467    ; 0x1d3
2158     19a0:   ea000003    b   19b4 <bl1_prepare_next_image+0x4c>
2159     19a4:   ee103f31    mrc 15, 0, r3, cr0, cr1, {1}
2160     19a8:   e3130a0f    tst r3, #61440  ; 0xf000
2161     19ac:   130031da    movwne  r3, #474    ; 0x1da
2162     19b0:   0afffff9    beq 199c <bl1_prepare_next_image+0x34>
2163     19b4:   e1a00007    mov r0, r7
2164     19b8:   e59f70f8    ldr r7, [pc, #248]  ; 1ab8 <bl1_prepare_next_image+0x150>
2165     19bc:   e1a01006    mov r1, r6
2166     19c0:   e585302c    str r3, [r5, #44]   ; 0x2c
2167     19c4:   eb0000e2    bl  1d54 <bl1_plat_set_ep_info>
2168     19c8:   e1a00006    mov r0, r6
2169     19cc:   eb00009d    bl  1c48 <cm_init_my_context>
2170     19d0:   e1a00004    mov r0, r4
2171     19d4:   eb0000a3    bl  1c68 <cm_prepare_el3_exit>
2172     19d8:   e0870284    add r0, r7, r4, lsl #5
2173     19dc:   ebffffd4    bl  1934 <cm_set_next_context>
2174     19e0:   e1a00004    mov r0, r4
2175     19e4:   ebffffb0    bl  18ac <smc_set_next_ctx>
2176     19e8:   e59f00cc    ldr r0, [pc, #204]  ; 1abc <bl1_prepare_next_image+0x154>
2177     19ec:   e3540000    cmp r4, #0
2178     19f0:   e59f10c8    ldr r1, [pc, #200]  ; 1ac0 <bl1_prepare_next_image+0x158>
2179     19f4:   e5902000    ldr r2, [r0]
2180     19f8:   e1a08000    mov r8, r0
2181     19fc:   e5913000    ldr r3, [r1]
2182     1a00:   e1a04001    mov r4, r1
2183     1a04:   e592c000    ldr ip, [r2]
2184     1a08:   e583c000    str ip, [r3]
2185     1a0c:   e592c004    ldr ip, [r2, #4]
2186     1a10:   e583c004    str ip, [r3, #4]
2187     1a14:   e592c008    ldr ip, [r2, #8]
2188     1a18:   e583c008    str ip, [r3, #8]
2189     1a1c:   e592c00c    ldr ip, [r2, #12]
2190     1a20:   e583c00c    str ip, [r3, #12]
2191     1a24:   e592c010    ldr ip, [r2, #16]
2192     1a28:   e583c080    str ip, [r3, #128]  ; 0x80
2193     1a2c:   e592c018    ldr ip, [r2, #24]
2194     1a30:   e5922014    ldr r2, [r2, #20]
2195     1a34:   e583c078    str ip, [r3, #120]  ; 0x78
2196     1a38:   e5832084    str r2, [r3, #132]  ; 0x84
2197     1a3c:   0a00000a    beq 1a6c <bl1_prepare_next_image+0x104>
2198     1a40:   ee113f11    mrc 15, 0, r3, cr1, cr1, {0}
2199     1a44:   e3833001    orr r3, r3, #1
2200     1a48:   ee013f11    mcr 15, 0, r3, cr1, cr1, {0}
2201     1a4c:   f57ff06f    isb sy
2202     1a50:   e597303c    ldr r3, [r7, #60]   ; 0x3c
2203     1a54:   ee013f10    mcr 15, 0, r3, cr1, cr0, {0}
2204     1a58:   f57ff06f    isb sy
2205     1a5c:   ee113f11    mrc 15, 0, r3, cr1, cr1, {0}
2206     1a60:   e3c33001    bic r3, r3, #1
2207     1a64:   ee013f11    mcr 15, 0, r3, cr1, cr1, {0}
2208     1a68:   f57ff06f    isb sy
2209     1a6c:   e3a01004    mov r1, #4
2210     1a70:   e59f0048    ldr r0, [pc, #72]   ; 1ac0 <bl1_prepare_next_image+0x158>
2211     1a74:   eb000872    bl  3c44 <flush_dcache_range>
2212     1a78:   e3a01090    mov r1, #144    ; 0x90
2213     1a7c:   e5940000    ldr r0, [r4]
2214     1a80:   eb00086f    bl  3c44 <flush_dcache_range>
2215     1a84:   e3a01004    mov r1, #4
2216     1a88:   e59f002c    ldr r0, [pc, #44]   ; 1abc <bl1_prepare_next_image+0x154>
2217     1a8c:   eb00086c    bl  3c44 <flush_dcache_range>
2218     1a90:   e3a01020    mov r1, #32
2219     1a94:   e5980000    ldr r0, [r8]
2220     1a98:   eb000869    bl  3c44 <flush_dcache_range>
2221     1a9c:   e3a03004    mov r3, #4
2222     1aa0:   e1a00006    mov r0, r6
2223     1aa4:   e5853004    str r3, [r5, #4]
2224     1aa8:   e8bd41f0    pop {r4, r5, r6, r7, r8, lr}
2225     1aac:   ea000134    b   1f84 <print_entry_point_info>
2226     1ab0:   000049a4    .word   0x000049a4
2227     1ab4:   00004b03    .word   0x00004b03
2228     1ab8:   0e02f160    .word   0x0e02f160
2229     1abc:   0e02f1a0    .word   0x0e02f1a0
2230     1ac0:   0e02f1a4    .word   0x0e02f1a4
2231 
2232 00001ac4 <errata_print_msg>:
2233     1ac4:   e3500002    cmp r0, #2
2234     1ac8:   e92d4007    push    {r0, r1, r2, lr}
2235     1acc:   859f2090    ldrhi   r2, [pc, #144]  ; 1b64 <errata_print_msg+0xa0>
2236     1ad0:   83a0104d    movhi   r1, #77 ; 0x4d
2237     1ad4:   8a000003    bhi 1ae8 <errata_print_msg+0x24>
2238     1ad8:   e3510000    cmp r1, #0
2239     1adc:   1a000003    bne 1af0 <errata_print_msg+0x2c>
2240     1ae0:   e59f2080    ldr r2, [pc, #128]  ; 1b68 <errata_print_msg+0xa4>
2241     1ae4:   e3a0104e    mov r1, #78 ; 0x4e
2242     1ae8:   e59f007c    ldr r0, [pc, #124]  ; 1b6c <errata_print_msg+0xa8>
2243     1aec:   eb0002fb    bl  26e0 <__assert>
2244     1af0:   e3520000    cmp r2, #0
2245     1af4:   e1a03002    mov r3, r2
2246     1af8:   03a0104f    moveq   r1, #79 ; 0x4f
2247     1afc:   059f206c    ldreq   r2, [pc, #108]  ; 1b70 <errata_print_msg+0xac>
2248     1b00:   0afffff8    beq 1ae8 <errata_print_msg+0x24>
2249     1b04:   e3500001    cmp r0, #1
2250     1b08:   0a000009    beq 1b34 <errata_print_msg+0x70>
2251     1b0c:   e3500002    cmp r0, #2
2252     1b10:   0a00000d    beq 1b4c <errata_print_msg+0x88>
2253     1b14:   e59f2058    ldr r2, [pc, #88]   ; 1b74 <errata_print_msg+0xb0>
2254     1b18:   e59f0058    ldr r0, [pc, #88]   ; 1b78 <errata_print_msg+0xb4>
2255     1b1c:   e58d2000    str r2, [sp]
2256     1b20:   e1a02001    mov r2, r1
2257     1b24:   e59f1050    ldr r1, [pc, #80]   ; 1b7c <errata_print_msg+0xb8>
2258     1b28:   eb000135    bl  2004 <tf_log>
2259     1b2c:   e28dd00c    add sp, sp, #12
2260     1b30:   e49df004    pop {pc}        ; (ldr pc, [sp], #4)
2261     1b34:   e59f2044    ldr r2, [pc, #68]   ; 1b80 <errata_print_msg+0xbc>
2262     1b38:   e59f0044    ldr r0, [pc, #68]   ; 1b84 <errata_print_msg+0xc0>
2263     1b3c:   e58d2000    str r2, [sp]
2264     1b40:   e1a02001    mov r2, r1
2265     1b44:   e59f1030    ldr r1, [pc, #48]   ; 1b7c <errata_print_msg+0xb8>
2266     1b48:   eafffff6    b   1b28 <errata_print_msg+0x64>
2267     1b4c:   e59f2034    ldr r2, [pc, #52]   ; 1b88 <errata_print_msg+0xc4>
2268     1b50:   e59f0034    ldr r0, [pc, #52]   ; 1b8c <errata_print_msg+0xc8>
2269     1b54:   e58d2000    str r2, [sp]
2270     1b58:   e1a02001    mov r2, r1
2271     1b5c:   e59f1018    ldr r1, [pc, #24]   ; 1b7c <errata_print_msg+0xb8>
2272     1b60:   eafffff0    b   1b28 <errata_print_msg+0x64>
2273     1b64:   00004b22    .word   0x00004b22
2274     1b68:   00004b62    .word   0x00004b62
2275     1b6c:   00004b49    .word   0x00004b49
2276     1b70:   00004b66    .word   0x00004b66
2277     1b74:   00004b94    .word   0x00004b94
2278     1b78:   00004b6d    .word   0x00004b6d
2279     1b7c:   00004b69    .word   0x00004b69
2280     1b80:   00004b98    .word   0x00004b98
2281     1b84:   00004ba0    .word   0x00004ba0
2282     1b88:   00004bee    .word   0x00004bee
2283     1b8c:   00004bc7    .word   0x00004bc7
2284 
2285 00001b90 <cm_init_context_common>:
2286     1b90:   e92d4070    push    {r4, r5, r6, lr}
2287     1b94:   e2504000    subs    r4, r0, #0
2288     1b98:   059f209c    ldreq   r2, [pc, #156]  ; 1c3c <cm_init_context_common+0xac>
2289     1b9c:   03a0103c    moveq   r1, #60 ; 0x3c
2290     1ba0:   0a000012    beq 1bf0 <cm_init_context_common+0x60>
2291     1ba4:   e1a05001    mov r5, r1
2292     1ba8:   e3a01020    mov r1, #32
2293     1bac:   e5956004    ldr r6, [r5, #4]
2294     1bb0:   eb000832    bl  3c80 <zero_normalmem>
2295     1bb4:   e2066001    and r6, r6, #1
2296     1bb8:   ee112f11    mrc 15, 0, r2, cr1, cr1, {0}
2297     1bbc:   e3560000    cmp r6, #0
2298     1bc0:   e3c22c01    bic r2, r2, #256    ; 0x100
2299     1bc4:   e595100c    ldr r1, [r5, #12]
2300     1bc8:   e3c22001    bic r2, r2, #1
2301     1bcc:   0a00000e    beq 1c0c <cm_init_context_common+0x7c>
2302     1bd0:   e5953004    ldr r3, [r5, #4]
2303     1bd4:   e3822001    orr r2, r2, #1
2304     1bd8:   e1a000a3    lsr r0, r3, #1
2305     1bdc:   e02004a1    eor r0, r0, r1, lsr #9
2306     1be0:   e3100001    tst r0, #1
2307     1be4:   0a000003    beq 1bf8 <cm_init_context_common+0x68>
2308     1be8:   e59f2050    ldr r2, [pc, #80]   ; 1c40 <cm_init_context_common+0xb0>
2309     1bec:   e3a01062    mov r1, #98 ; 0x62
2310     1bf0:   e59f004c    ldr r0, [pc, #76]   ; 1c44 <cm_init_context_common+0xb4>
2311     1bf4:   eb0002b9    bl  26e0 <__assert>
2312     1bf8:   e1a03c03    lsl r3, r3, #24
2313     1bfc:   e2033402    and r3, r3, #33554432   ; 0x2000000
2314     1c00:   e38338c5    orr r3, r3, #12910592   ; 0xc50000
2315     1c04:   e3833038    orr r3, r3, #56 ; 0x38
2316     1c08:   e584301c    str r3, [r4, #28]
2317     1c0c:   e201301f    and r3, r1, #31
2318     1c10:   e1a00004    mov r0, r4
2319     1c14:   e353001a    cmp r3, #26
2320     1c18:   e5953008    ldr r3, [r5, #8]
2321     1c1c:   03822c01    orreq   r2, r2, #256    ; 0x100
2322     1c20:   e5843010    str r3, [r4, #16]
2323     1c24:   e5842014    str r2, [r4, #20]
2324     1c28:   e3a02010    mov r2, #16
2325     1c2c:   e5841018    str r1, [r4, #24]
2326     1c30:   e2851014    add r1, r5, #20
2327     1c34:   e8bd4070    pop {r4, r5, r6, lr}
2328     1c38:   ea0002bc    b   2730 <memcpy>
2329     1c3c:   00004bf7    .word   0x00004bf7
2330     1c40:   00004c22    .word   0x00004c22
2331     1c44:   00004bfb    .word   0x00004bfb
2332 
2333 00001c48 <cm_init_my_context>:
2334     1c48:   e92d4010    push    {r4, lr}
2335     1c4c:   e1a04000    mov r4, r0
2336     1c50:   e5900004    ldr r0, [r0, #4]
2337     1c54:   e2000001    and r0, r0, #1
2338     1c58:   ebffff28    bl  1900 <cm_get_context>
2339     1c5c:   e1a01004    mov r1, r4
2340     1c60:   e8bd4010    pop {r4, lr}
2341     1c64:   eaffffc9    b   1b90 <cm_init_context_common>
2342 
2343 00001c68 <cm_prepare_el3_exit>:
2344     1c68:   e92d4010    push    {r4, lr}
2345     1c6c:   e1a04000    mov r4, r0
2346     1c70:   ebffff22    bl  1900 <cm_get_context>
2347     1c74:   e3500000    cmp r0, #0
2348     1c78:   1a000003    bne 1c8c <cm_prepare_el3_exit+0x24>
2349     1c7c:   e59f20bc    ldr r2, [pc, #188]  ; 1d40 <cm_prepare_el3_exit+0xd8>
2350     1c80:   e3a010b5    mov r1, #181    ; 0xb5
2351     1c84:   e59f00b8    ldr r0, [pc, #184]  ; 1d44 <cm_prepare_el3_exit+0xdc>
2352     1c88:   eb000294    bl  26e0 <__assert>
2353     1c8c:   e3540001    cmp r4, #1
2354     1c90:   18bd8010    popne   {r4, pc}
2355     1c94:   e5903014    ldr r3, [r0, #20]
2356     1c98:   e2133c01    ands    r3, r3, #256    ; 0x100
2357     1c9c:   0a00000d    beq 1cd8 <cm_prepare_el3_exit+0x70>
2358     1ca0:   e590201c    ldr r2, [r0, #28]
2359     1ca4:   ee113f11    mrc 15, 0, r3, cr1, cr1, {0}
2360     1ca8:   e3833001    orr r3, r3, #1
2361     1cac:   ee013f11    mcr 15, 0, r3, cr1, cr1, {0}
2362     1cb0:   f57ff06f    isb sy
2363     1cb4:   e59f308c    ldr r3, [pc, #140]  ; 1d48 <cm_prepare_el3_exit+0xe0>
2364     1cb8:   e1823003    orr r3, r2, r3
2365     1cbc:   ee813f10    mcr 15, 4, r3, cr1, cr0, {0}
2366     1cc0:   f57ff06f    isb sy
2367     1cc4:   ee113f11    mrc 15, 0, r3, cr1, cr1, {0}
2368     1cc8:   e3c33001    bic r3, r3, #1
2369     1ccc:   ee013f11    mcr 15, 0, r3, cr1, cr1, {0}
2370     1cd0:   f57ff06f    isb sy
2371     1cd4:   e8bd8010    pop {r4, pc}
2372     1cd8:   ee102f31    mrc 15, 0, r2, cr0, cr1, {1}
2373     1cdc:   e3120a0f    tst r2, #61440  ; 0xf000
2374     1ce0:   08bd8010    popeq   {r4, pc}
2375     1ce4:   ee112f11    mrc 15, 0, r2, cr1, cr1, {0}
2376     1ce8:   e3822001    orr r2, r2, #1
2377     1cec:   ee012f11    mcr 15, 0, r2, cr1, cr1, {0}
2378     1cf0:   f57ff06f    isb sy
2379     1cf4:   ee813f11    mcr 15, 4, r3, cr1, cr1, {0}
2380     1cf8:   e30323ff    movw    r2, #13311  ; 0x33ff
2381     1cfc:   ee812f51    mcr 15, 4, r2, cr1, cr1, {2}
2382     1d00:   e3a02003    mov r2, #3
2383     1d04:   ee8e2f11    mcr 15, 4, r2, cr14, cr1, {0}
2384     1d08:   e3a00000    mov r0, #0
2385     1d0c:   e3a01000    mov r1, #0
2386     1d10:   ec410f4e    mcrr    15, 4, r0, r1, cr14
2387     1d14:   ee102f10    mrc 15, 0, r2, cr0, cr0, {0}
2388     1d18:   ee802f10    mcr 15, 4, r2, cr0, cr0, {0}
2389     1d1c:   ee102fb0    mrc 15, 0, r2, cr0, cr0, {5}
2390     1d20:   ee802fb0    mcr 15, 4, r2, cr0, cr0, {5}
2391     1d24:   ec410f62    mcrr    15, 6, r0, r1, cr2
2392     1d28:   ee192f1c    mrc 15, 0, r2, cr9, cr12, {0}
2393     1d2c:   e7e425d2    ubfx    r2, r2, #11, #5
2394     1d30:   ee812f31    mcr 15, 4, r2, cr1, cr1, {1}
2395     1d34:   ee813f71    mcr 15, 4, r3, cr1, cr1, {3}
2396     1d38:   ee8e3f32    mcr 15, 4, r3, cr14, cr2, {1}
2397     1d3c:   eaffffdf    b   1cc0 <cm_prepare_el3_exit+0x58>
2398     1d40:   00004bf7    .word   0x00004bf7
2399     1d44:   00004bfb    .word   0x00004bfb
2400     1d48:   30c50818    .word   0x30c50818
2401 
2402 00001d4c <bl1_plat_get_next_image_id>:
2403     1d4c:   e3a00001    mov r0, #1
2404     1d50:   e12fff1e    bx  lr
2405 
2406 00001d54 <bl1_plat_set_ep_info>:
2407     1d54:   e12fff1e    bx  lr
2408 
2409 00001d58 <bl1_plat_handle_pre_image_load>:
2410     1d58:   e3a00000    mov r0, #0
2411     1d5c:   e12fff1e    bx  lr
2412 
2413 00001d60 <bl1_plat_get_image_desc>:
2414     1d60:   e59f0000    ldr r0, [pc]    ; 1d68 <bl1_plat_get_image_desc+0x8>
2415     1d64:   e12fff1e    bx  lr
2416     1d68:   0e02e000    .word   0x0e02e000
2417 
2418 00001d6c <bl1_plat_handle_post_image_load>:
2419     1d6c:   e3500001    cmp r0, #1
2420     1d70:   1a000011    bne 1dbc <bl1_plat_handle_post_image_load+0x50>
2421     1d74:   e92d4070    push    {r4, r5, r6, lr}
2422     1d78:   ebfffff8    bl  1d60 <bl1_plat_get_image_desc>
2423     1d7c:   e2505000    subs    r5, r0, #0
2424     1d80:   1a000003    bne 1d94 <bl1_plat_handle_post_image_load+0x28>
2425     1d84:   e59f2038    ldr r2, [pc, #56]   ; 1dc4 <bl1_plat_handle_post_image_load+0x58>
2426     1d88:   e3a0105c    mov r1, #92 ; 0x5c
2427     1d8c:   e59f0034    ldr r0, [pc, #52]   ; 1dc8 <bl1_plat_handle_post_image_load+0x5c>
2428     1d90:   eb000252    bl  26e0 <__assert>
2429     1d94:   ebfffddd    bl  1510 <bl1_plat_sec_mem_layout>
2430     1d98:   e5904000    ldr r4, [r0]
2431     1d9c:   e1a01004    mov r1, r4
2432     1da0:   ebfffe24    bl  1638 <bl1_init_bl2_mem_layout>
2433     1da4:   e1a01004    mov r1, r4
2434     1da8:   e59f001c    ldr r0, [pc, #28]   ; 1dcc <bl1_plat_handle_post_image_load+0x60>
2435     1dac:   e5854038    str r4, [r5, #56]   ; 0x38
2436     1db0:   eb000093    bl  2004 <tf_log>
2437     1db4:   e3a00000    mov r0, #0
2438     1db8:   e8bd8070    pop {r4, r5, r6, pc}
2439     1dbc:   e3a00000    mov r0, #0
2440     1dc0:   e12fff1e    bx  lr
2441     1dc4:   00004c77    .word   0x00004c77
2442     1dc8:   00004cb0    .word   0x00004cb0
2443     1dcc:   00004c8a    .word   0x00004c8a
2444 
2445 00001dd0 <load_auth_image>:
2446     1dd0:   e92d47f0    push    {r4, r5, r6, r7, r8, r9, sl, lr}
2447     1dd4:   e1a05000    mov r5, r0
2448     1dd8:   e59f817c    ldr r8, [pc, #380]  ; 1f5c <load_auth_image+0x18c>
2449     1ddc:   e24dd018    sub sp, sp, #24
2450     1de0:   e1a06001    mov r6, r1
2451     1de4:   e59f9174    ldr r9, [pc, #372]  ; 1f60 <load_auth_image+0x190>
2452     1de8:   e59fa174    ldr sl, [pc, #372]  ; 1f64 <load_auth_image+0x194>
2453     1dec:   e3560000    cmp r6, #0
2454     1df0:   1a000003    bne 1e04 <load_auth_image+0x34>
2455     1df4:   e59f216c    ldr r2, [pc, #364]  ; 1f68 <load_auth_image+0x198>
2456     1df8:   e3a010dc    mov r1, #220    ; 0xdc
2457     1dfc:   e59f0168    ldr r0, [pc, #360]  ; 1f6c <load_auth_image+0x19c>
2458     1e00:   eb000236    bl  26e0 <__assert>
2459     1e04:   e5d63001    ldrb    r3, [r6, #1]
2460     1e08:   e3530001    cmp r3, #1
2461     1e0c:   959f215c    ldrls   r2, [pc, #348]  ; 1f70 <load_auth_image+0x1a0>
2462     1e10:   93a010dd    movls   r1, #221    ; 0xdd
2463     1e14:   9afffff8    bls 1dfc <load_auth_image+0x2c>
2464     1e18:   e28d200c    add r2, sp, #12
2465     1e1c:   e28d1004    add r1, sp, #4
2466     1e20:   e5967008    ldr r7, [r6, #8]
2467     1e24:   e1a00005    mov r0, r5
2468     1e28:   ebfffd7e    bl  1428 <plat_get_image_source>
2469     1e2c:   e2504000    subs    r4, r0, #0
2470     1e30:   0a000007    beq 1e54 <load_auth_image+0x84>
2471     1e34:   e59f0138    ldr r0, [pc, #312]  ; 1f74 <load_auth_image+0x1a4>
2472     1e38:   e1a02004    mov r2, r4
2473     1e3c:   e1a01005    mov r1, r5
2474     1e40:   eb00006f    bl  2004 <tf_log>
2475     1e44:   eb000153    bl  2398 <plat_try_next_boot_source>
2476     1e48:   e3500000    cmp r0, #0
2477     1e4c:   1affffe6    bne 1dec <load_auth_image+0x1c>
2478     1e50:   ea000029    b   1efc <load_auth_image+0x12c>
2479     1e54:   e28d2008    add r2, sp, #8
2480     1e58:   e59d0004    ldr r0, [sp, #4]
2481     1e5c:   e59d100c    ldr r1, [sp, #12]
2482     1e60:   ebfffa17    bl  6c4 <io_open>
2483     1e64:   e2504000    subs    r4, r0, #0
2484     1e68:   0a000003    beq 1e7c <load_auth_image+0xac>
2485     1e6c:   e1a02004    mov r2, r4
2486     1e70:   e1a01005    mov r1, r5
2487     1e74:   e59f00fc    ldr r0, [pc, #252]  ; 1f78 <load_auth_image+0x1a8>
2488     1e78:   eafffff0    b   1e40 <load_auth_image+0x70>
2489     1e7c:   e1a02007    mov r2, r7
2490     1e80:   e1a01005    mov r1, r5
2491     1e84:   e1a00008    mov r0, r8
2492     1e88:   eb00005d    bl  2004 <tf_log>
2493     1e8c:   e28d1010    add r1, sp, #16
2494     1e90:   e59d0008    ldr r0, [sp, #8]
2495     1e94:   ebfffa6b    bl  848 <io_size>
2496     1e98:   e2504000    subs    r4, r0, #0
2497     1e9c:   1a000002    bne 1eac <load_auth_image+0xdc>
2498     1ea0:   e59d2010    ldr r2, [sp, #16]
2499     1ea4:   e3520000    cmp r2, #0
2500     1ea8:   1a000004    bne 1ec0 <load_auth_image+0xf0>
2501     1eac:   e1a02004    mov r2, r4
2502     1eb0:   e1a01005    mov r1, r5
2503     1eb4:   e1a00009    mov r0, r9
2504     1eb8:   eb000051    bl  2004 <tf_log>
2505     1ebc:   ea000006    b   1edc <load_auth_image+0x10c>
2506     1ec0:   e5963010    ldr r3, [r6, #16]
2507     1ec4:   e1520003    cmp r2, r3
2508     1ec8:   9a00000e    bls 1f08 <load_auth_image+0x138>
2509     1ecc:   e1a01005    mov r1, r5
2510     1ed0:   e59f00a4    ldr r0, [pc, #164]  ; 1f7c <load_auth_image+0x1ac>
2511     1ed4:   e3e0401a    mvn r4, #26
2512     1ed8:   eb000049    bl  2004 <tf_log>
2513     1edc:   e59d0008    ldr r0, [sp, #8]
2514     1ee0:   ebfffa8d    bl  91c <io_close>
2515     1ee4:   e59d0004    ldr r0, [sp, #4]
2516     1ee8:   ebfff9dd    bl  664 <io_dev_close>
2517     1eec:   e3540000    cmp r4, #0
2518     1ef0:   1affffd3    bne 1e44 <load_auth_image+0x74>
2519     1ef4:   e1c600d8    ldrd    r0, [r6, #8]
2520     1ef8:   eb000751    bl  3c44 <flush_dcache_range>
2521     1efc:   e1a00004    mov r0, r4
2522     1f00:   e28dd018    add sp, sp, #24
2523     1f04:   e8bd87f0    pop {r4, r5, r6, r7, r8, r9, sl, pc}
2524     1f08:   e28d3014    add r3, sp, #20
2525     1f0c:   e1a01007    mov r1, r7
2526     1f10:   e586200c    str r2, [r6, #12]
2527     1f14:   e59d0008    ldr r0, [sp, #8]
2528     1f18:   ebfffa62    bl  8a8 <io_read>
2529     1f1c:   e2504000    subs    r4, r0, #0
2530     1f20:   1a000003    bne 1f34 <load_auth_image+0x164>
2531     1f24:   e59d3010    ldr r3, [sp, #16]
2532     1f28:   e59d2014    ldr r2, [sp, #20]
2533     1f2c:   e1520003    cmp r2, r3
2534     1f30:   2a000003    bcs 1f44 <load_auth_image+0x174>
2535     1f34:   e1a02004    mov r2, r4
2536     1f38:   e1a01005    mov r1, r5
2537     1f3c:   e1a0000a    mov r0, sl
2538     1f40:   eaffffdc    b   1eb8 <load_auth_image+0xe8>
2539     1f44:   e0873003    add r3, r7, r3
2540     1f48:   e1a02007    mov r2, r7
2541     1f4c:   e59f002c    ldr r0, [pc, #44]   ; 1f80 <load_auth_image+0x1b0>
2542     1f50:   e1a01005    mov r1, r5
2543     1f54:   eb00002a    bl  2004 <tf_log>
2544     1f58:   eaffffdf    b   1edc <load_auth_image+0x10c>
2545     1f5c:   00004d4e    .word   0x00004d4e
2546     1f60:   00004cce    .word   0x00004cce
2547     1f64:   00004d93    .word   0x00004d93
2548     1f68:   00004d05    .word   0x00004d05
2549     1f6c:   00004d18    .word   0x00004d18
2550     1f70:   00004d2b    .word   0x00004d2b
2551     1f74:   000042c0    .word   0x000042c0
2552     1f78:   000042f1    .word   0x000042f1
2553     1f7c:   00004d72    .word   0x00004d72
2554     1f80:   00004db5    .word   0x00004db5
2555 
2556 00001f84 <print_entry_point_info>:
2557     1f84:   e92d4070    push    {r4, r5, r6, lr}
2558     1f88:   e1a04000    mov r4, r0
2559     1f8c:   e59f0058    ldr r0, [pc, #88]   ; 1fec <print_entry_point_info+0x68>
2560     1f90:   e3a05000    mov r5, #0
2561     1f94:   e5941008    ldr r1, [r4, #8]
2562     1f98:   eb000019    bl  2004 <tf_log>
2563     1f9c:   e59f004c    ldr r0, [pc, #76]   ; 1ff0 <print_entry_point_info+0x6c>
2564     1fa0:   e594100c    ldr r1, [r4, #12]
2565     1fa4:   eb000016    bl  2004 <tf_log>
2566     1fa8:   e1a03005    mov r3, r5
2567     1fac:   e59f0040    ldr r0, [pc, #64]   ; 1ff4 <print_entry_point_info+0x70>
2568     1fb0:   e5942014    ldr r2, [r4, #20]
2569     1fb4:   eb000012    bl  2004 <tf_log>
2570     1fb8:   e1a03005    mov r3, r5
2571     1fbc:   e59f0034    ldr r0, [pc, #52]   ; 1ff8 <print_entry_point_info+0x74>
2572     1fc0:   e5942018    ldr r2, [r4, #24]
2573     1fc4:   eb00000e    bl  2004 <tf_log>
2574     1fc8:   e1a03005    mov r3, r5
2575     1fcc:   e59f0028    ldr r0, [pc, #40]   ; 1ffc <print_entry_point_info+0x78>
2576     1fd0:   e594201c    ldr r2, [r4, #28]
2577     1fd4:   eb00000a    bl  2004 <tf_log>
2578     1fd8:   e1a03005    mov r3, r5
2579     1fdc:   e59f001c    ldr r0, [pc, #28]   ; 2000 <print_entry_point_info+0x7c>
2580     1fe0:   e5942020    ldr r2, [r4, #32]
2581     1fe4:   e8bd4070    pop {r4, r5, r6, lr}
2582     1fe8:   ea000005    b   2004 <tf_log>
2583     1fec:   00004dd3    .word   0x00004dd3
2584     1ff0:   00004dee    .word   0x00004dee
2585     1ff4:   00004dfc    .word   0x00004dfc
2586     1ff8:   00004e13    .word   0x00004e13
2587     1ffc:   00004e2a    .word   0x00004e2a
2588     2000:   00004e41    .word   0x00004e41
2589 
2590 00002004 <tf_log>:
2591     2004:   e92d000f    push    {r0, r1, r2, r3}
2592     2008:   e92d4013    push    {r0, r1, r4, lr}
2593     200c:   e59d4010    ldr r4, [sp, #16]
2594     2010:   e5d40000    ldrb    r0, [r4]
2595     2014:   e2403001    sub r3, r0, #1
2596     2018:   e3530031    cmp r3, #49 ; 0x31
2597     201c:   859f2068    ldrhi   r2, [pc, #104]  ; 208c <tf_log+0x88>
2598     2020:   83a0101f    movhi   r1, #31
2599     2024:   8a000006    bhi 2044 <tf_log+0x40>
2600     2028:   e3a0200a    mov r2, #10
2601     202c:   e733f210    udiv    r3, r0, r2
2602     2030:   e0630392    mls r3, r2, r3, r0
2603     2034:   e3530000    cmp r3, #0
2604     2038:   0a000003    beq 204c <tf_log+0x48>
2605     203c:   e59f204c    ldr r2, [pc, #76]   ; 2090 <tf_log+0x8c>
2606     2040:   e3a01020    mov r1, #32
2607     2044:   e59f0048    ldr r0, [pc, #72]   ; 2094 <tf_log+0x90>
2608     2048:   eb0001a4    bl  26e0 <__assert>
2609     204c:   e59f3044    ldr r3, [pc, #68]   ; 2098 <tf_log+0x94>
2610     2050:   e5933000    ldr r3, [r3]
2611     2054:   e1530000    cmp r3, r0
2612     2058:   3a000007    bcc 207c <tf_log+0x78>
2613     205c:   eb0000cf    bl  23a0 <plat_log_get_prefix>
2614     2060:   e3500000    cmp r0, #0
2615     2064:   0a000000    beq 206c <tf_log+0x68>
2616     2068:   eb000037    bl  214c <tf_string_print>
2617     206c:   e28d1014    add r1, sp, #20
2618     2070:   e2840001    add r0, r4, #1
2619     2074:   e58d1004    str r1, [sp, #4]
2620     2078:   eb000042    bl  2188 <tf_vprintf>
2621     207c:   e28dd008    add sp, sp, #8
2622     2080:   e8bd4010    pop {r4, lr}
2623     2084:   e28dd010    add sp, sp, #16
2624     2088:   e12fff1e    bx  lr
2625     208c:   00004e58    .word   0x00004e58
2626     2090:   00004e94    .word   0x00004e94
2627     2094:   00004e84    .word   0x00004e84
2628     2098:   0e02e044    .word   0x0e02e044
2629 
2630 0000209c <unsigned_num_print>:
2631     209c:   e92d4ff0    push    {r4, r5, r6, r7, r8, r9, sl, fp, lr}
2632     20a0:   e24dd01c    sub sp, sp, #28
2633     20a4:   e59d5040    ldr r5, [sp, #64]   ; 0x40
2634     20a8:   e1a06000    mov r6, r0
2635     20ac:   e1a07001    mov r7, r1
2636     20b0:   e1a0b003    mov fp, r3
2637     20b4:   e28da004    add sl, sp, #4
2638     20b8:   e3a04000    mov r4, #0
2639     20bc:   e1a08002    mov r8, r2
2640     20c0:   e3a09000    mov r9, #0
2641     20c4:   e1a02008    mov r2, r8
2642     20c8:   e1a03009    mov r3, r9
2643     20cc:   e1a00006    mov r0, r6
2644     20d0:   e1a01007    mov r1, r7
2645     20d4:   eb000726    bl  3d74 <__aeabi_uldivmod>
2646     20d8:   e6ef3072    uxtb    r3, r2
2647     20dc:   e3520009    cmp r2, #9
2648     20e0:   e2844001    add r4, r4, #1
2649     20e4:   d2833030    addle   r3, r3, #48 ; 0x30
2650     20e8:   c2833057    addgt   r3, r3, #87 ; 0x57
2651     20ec:   e6ef3073    uxtb    r3, r3
2652     20f0:   e1590007    cmp r9, r7
2653     20f4:   01580006    cmpeq   r8, r6
2654     20f8:   e4ca3001    strb    r3, [sl], #1
2655     20fc:   9a000006    bls 211c <unsigned_num_print+0x80>
2656     2100:   e3550000    cmp r5, #0
2657     2104:   ca00000a    bgt 2134 <unsigned_num_print+0x98>
2658     2108:   e28d5004    add r5, sp, #4
2659     210c:   e2544001    subs    r4, r4, #1
2660     2110:   2a00000a    bcs 2140 <unsigned_num_print+0xa4>
2661     2114:   e28dd01c    add sp, sp, #28
2662     2118:   e8bd8ff0    pop {r4, r5, r6, r7, r8, r9, sl, fp, pc}
2663     211c:   e1a06000    mov r6, r0
2664     2120:   e1a07001    mov r7, r1
2665     2124:   eaffffe6    b   20c4 <unsigned_num_print+0x28>
2666     2128:   e1a0000b    mov r0, fp
2667     212c:   e2455001    sub r5, r5, #1
2668     2130:   eb000195    bl  278c <putchar>
2669     2134:   e1550004    cmp r5, r4
2670     2138:   cafffffa    bgt 2128 <unsigned_num_print+0x8c>
2671     213c:   eafffff1    b   2108 <unsigned_num_print+0x6c>
2672     2140:   e7d50004    ldrb    r0, [r5, r4]
2673     2144:   eb000190    bl  278c <putchar>
2674     2148:   eaffffef    b   210c <unsigned_num_print+0x70>
2675 
2676 0000214c <tf_string_print>:
2677     214c:   e3500000    cmp r0, #0
2678     2150:   e92d4010    push    {r4, lr}
2679     2154:   12404001    subne   r4, r0, #1
2680     2158:   1a000004    bne 2170 <tf_string_print+0x24>
2681     215c:   e59f201c    ldr r2, [pc, #28]   ; 2180 <tf_string_print+0x34>
2682     2160:   e3a0101c    mov r1, #28
2683     2164:   e59f0018    ldr r0, [pc, #24]   ; 2184 <tf_string_print+0x38>
2684     2168:   eb00015c    bl  26e0 <__assert>
2685     216c:   eb000186    bl  278c <putchar>
2686     2170:   e5f40001    ldrb    r0, [r4, #1]!
2687     2174:   e3500000    cmp r0, #0
2688     2178:   1afffffb    bne 216c <tf_string_print+0x20>
2689     217c:   e8bd8010    pop {r4, pc}
2690     2180:   00004ea8    .word   0x00004ea8
2691     2184:   00004eac    .word   0x00004eac
2692 
2693 00002188 <tf_vprintf>:
2694     2188:   e92d4ff7    push    {r0, r1, r2, r4, r5, r6, r7, r8, r9, sl, fp, lr}
2695     218c:   e1a05000    mov r5, r0
2696     2190:   e1a04001    mov r4, r1
2697     2194:   e3a07000    mov r7, #0
2698     2198:   e3a0a00a    mov sl, #10
2699     219c:   e5d50000    ldrb    r0, [r5]
2700     21a0:   e3500000    cmp r0, #0
2701     21a4:   0a00000f    beq 21e8 <tf_vprintf+0x60>
2702     21a8:   e3500025    cmp r0, #37 ; 0x25
2703     21ac:   e2855001    add r5, r5, #1
2704     21b0:   1a000069    bne 235c <tf_vprintf+0x1d4>
2705     21b4:   e3a06000    mov r6, #0
2706     21b8:   e1a02006    mov r2, r6
2707     21bc:   ea000000    b   21c4 <tf_vprintf+0x3c>
2708     21c0:   e3a07030    mov r7, #48 ; 0x30
2709     21c4:   e5d53000    ldrb    r3, [r5]
2710     21c8:   e3530070    cmp r3, #112    ; 0x70
2711     21cc:   0a000033    beq 22a0 <tf_vprintf+0x118>
2712     21d0:   8a00000c    bhi 2208 <tf_vprintf+0x80>
2713     21d4:   e3530064    cmp r3, #100    ; 0x64
2714     21d8:   0a000018    beq 2240 <tf_vprintf+0xb8>
2715     21dc:   8a000003    bhi 21f0 <tf_vprintf+0x68>
2716     21e0:   e3530030    cmp r3, #48 ; 0x30
2717     21e4:   0a000052    beq 2334 <tf_vprintf+0x1ac>
2718     21e8:   e28dd00c    add sp, sp, #12
2719     21ec:   e8bd8ff0    pop {r4, r5, r6, r7, r8, r9, sl, fp, pc}
2720     21f0:   e3530069    cmp r3, #105    ; 0x69
2721     21f4:   0a000011    beq 2240 <tf_vprintf+0xb8>
2722     21f8:   e353006c    cmp r3, #108    ; 0x6c
2723     21fc:   1afffff9    bne 21e8 <tf_vprintf+0x60>
2724     2200:   e2822001    add r2, r2, #1
2725     2204:   ea00000b    b   2238 <tf_vprintf+0xb0>
2726     2208:   e3530075    cmp r3, #117    ; 0x75
2727     220c:   0a00003c    beq 2304 <tf_vprintf+0x17c>
2728     2210:   8a000004    bhi 2228 <tf_vprintf+0xa0>
2729     2214:   e3530073    cmp r3, #115    ; 0x73
2730     2218:   1afffff2    bne 21e8 <tf_vprintf+0x60>
2731     221c:   e4940004    ldr r0, [r4], #4
2732     2220:   ebffffc9    bl  214c <tf_string_print>
2733     2224:   ea00001b    b   2298 <tf_vprintf+0x110>
2734     2228:   e3530078    cmp r3, #120    ; 0x78
2735     222c:   0a000028    beq 22d4 <tf_vprintf+0x14c>
2736     2230:   e353007a    cmp r3, #122    ; 0x7a
2737     2234:   1affffeb    bne 21e8 <tf_vprintf+0x60>
2738     2238:   e2855001    add r5, r5, #1
2739     223c:   eaffffe0    b   21c4 <tf_vprintf+0x3c>
2740     2240:   e3520001    cmp r2, #1
2741     2244:   c2844007    addgt   r4, r4, #7
2742     2248:   d5948000    ldrle   r8, [r4]
2743     224c:   d284b004    addle   fp, r4, #4
2744     2250:   c3c4b007    bicgt   fp, r4, #7
2745     2254:   c0cb80d8    ldrdgt  r8, [fp], #8
2746     2258:   d1a09fc8    asrle   r9, r8, #31
2747     225c:   e3580000    cmp r8, #0
2748     2260:   e2d93000    sbcs    r3, r9, #0
2749     2264:   a1a00008    movge   r0, r8
2750     2268:   a1a01009    movge   r1, r9
2751     226c:   aa000004    bge 2284 <tf_vprintf+0xfc>
2752     2270:   e3a0002d    mov r0, #45 ; 0x2d
2753     2274:   e2466001    sub r6, r6, #1
2754     2278:   eb000143    bl  278c <putchar>
2755     227c:   e2780000    rsbs    r0, r8, #0
2756     2280:   e2e91000    rsc r1, r9, #0
2757     2284:   e1a03007    mov r3, r7
2758     2288:   e3a0200a    mov r2, #10
2759     228c:   e58d6000    str r6, [sp]
2760     2290:   e1a0400b    mov r4, fp
2761     2294:   ebffff80    bl  209c <unsigned_num_print>
2762     2298:   e2855001    add r5, r5, #1
2763     229c:   eaffffbe    b   219c <tf_vprintf+0x14>
2764     22a0:   e4948004    ldr r8, [r4], #4
2765     22a4:   e3580000    cmp r8, #0
2766     22a8:   0a000002    beq 22b8 <tf_vprintf+0x130>
2767     22ac:   e59f00b0    ldr r0, [pc, #176]  ; 2364 <tf_vprintf+0x1dc>
2768     22b0:   e2466002    sub r6, r6, #2
2769     22b4:   ebffffa4    bl  214c <tf_string_print>
2770     22b8:   e1a03007    mov r3, r7
2771     22bc:   e3a02010    mov r2, #16
2772     22c0:   e58d6000    str r6, [sp]
2773     22c4:   e1a00008    mov r0, r8
2774     22c8:   e3a01000    mov r1, #0
2775     22cc:   ebffff72    bl  209c <unsigned_num_print>
2776     22d0:   eafffff0    b   2298 <tf_vprintf+0x110>
2777     22d4:   e3520001    cmp r2, #1
2778     22d8:   e3a02010    mov r2, #16
2779     22dc:   c2844007    addgt   r4, r4, #7
2780     22e0:   d4940004    ldrle   r0, [r4], #4
2781     22e4:   d3a01000    movle   r1, #0
2782     22e8:   c3c43007    bicgt   r3, r4, #7
2783     22ec:   c1a04003    movgt   r4, r3
2784     22f0:   c5931004    ldrgt   r1, [r3, #4]
2785     22f4:   e1a03007    mov r3, r7
2786     22f8:   c4940008    ldrgt   r0, [r4], #8
2787     22fc:   e58d6000    str r6, [sp]
2788     2300:   eafffff1    b   22cc <tf_vprintf+0x144>
2789     2304:   e3520001    cmp r2, #1
2790     2308:   e3a0200a    mov r2, #10
2791     230c:   c2844007    addgt   r4, r4, #7
2792     2310:   d4940004    ldrle   r0, [r4], #4
2793     2314:   d3a01000    movle   r1, #0
2794     2318:   c3c43007    bicgt   r3, r4, #7
2795     231c:   c1a04003    movgt   r4, r3
2796     2320:   c5931004    ldrgt   r1, [r3, #4]
2797     2324:   e1a03007    mov r3, r7
2798     2328:   c4940008    ldrgt   r0, [r4], #8
2799     232c:   e58d6000    str r6, [sp]
2800     2330:   eaffffe5    b   22cc <tf_vprintf+0x144>
2801     2334:   e2851001    add r1, r5, #1
2802     2338:   e3a06000    mov r6, #0
2803     233c:   e1a05001    mov r5, r1
2804     2340:   e4d13001    ldrb    r3, [r1], #1
2805     2344:   e2433030    sub r3, r3, #48 ; 0x30
2806     2348:   e6ef0073    uxtb    r0, r3
2807     234c:   e3500009    cmp r0, #9
2808     2350:   8affff9a    bhi 21c0 <tf_vprintf+0x38>
2809     2354:   e026369a    mla r6, sl, r6, r3
2810     2358:   eafffff7    b   233c <tf_vprintf+0x1b4>
2811     235c:   eb00010a    bl  278c <putchar>
2812     2360:   eaffff8d    b   219c <tf_vprintf+0x14>
2813     2364:   00004ebf    .word   0x00004ebf
2814 
2815 00002368 <tf_printf>:
2816     2368:   e92d000f    push    {r0, r1, r2, r3}
2817     236c:   e92d4007    push    {r0, r1, r2, lr}
2818     2370:   e28d1014    add r1, sp, #20
2819     2374:   e59d0010    ldr r0, [sp, #16]
2820     2378:   e58d1004    str r1, [sp, #4]
2821     237c:   ebffff81    bl  2188 <tf_vprintf>
2822     2380:   e28dd00c    add sp, sp, #12
2823     2384:   e49de004    pop {lr}        ; (ldr lr, [sp], #4)
2824     2388:   e28dd010    add sp, sp, #16
2825     238c:   e12fff1e    bx  lr
2826 
2827 00002390 <plat_error_handler>:
2828     2390:   e320f003    wfi
2829     2394:   eafffffd    b   2390 <plat_error_handler>
2830 
2831 00002398 <plat_try_next_boot_source>:
2832     2398:   e3a00000    mov r0, #0
2833     239c:   e12fff1e    bx  lr
2834 
2835 000023a0 <plat_log_get_prefix>:
2836     23a0:   e3500032    cmp r0, #50 ; 0x32
2837     23a4:   e3a0300a    mov r3, #10
2838     23a8:   23a00032    movcs   r0, #50 ; 0x32
2839     23ac:   e350000a    cmp r0, #10
2840     23b0:   33a0000a    movcc   r0, #10
2841     23b4:   e730f310    udiv    r0, r0, r3
2842     23b8:   e59f3008    ldr r3, [pc, #8]    ; 23c8 <plat_log_get_prefix+0x28>
2843     23bc:   e2400001    sub r0, r0, #1
2844     23c0:   e7930100    ldr r0, [r3, r0, lsl #2]
2845     23c4:   e12fff1e    bx  lr
2846     23c8:   00004ec4    .word   0x00004ec4
2847 
2848 000023cc <__udivmoddi4>:
2849     23cc:   e92d4ff0    push    {r4, r5, r6, r7, r8, r9, sl, fp, lr}
2850     23d0:   e24dd014    sub sp, sp, #20
2851     23d4:   e59de038    ldr lr, [sp, #56]   ; 0x38
2852     23d8:   e1cd00f0    strd    r0, [sp]
2853     23dc:   e59d6004    ldr r6, [sp, #4]
2854     23e0:   e3560000    cmp r6, #0
2855     23e4:   1a00000f    bne 2428 <__udivmoddi4+0x5c>
2856     23e8:   e3530000    cmp r3, #0
2857     23ec:   1a000007    bne 2410 <__udivmoddi4+0x44>
2858     23f0:   e35e0000    cmp lr, #0
2859     23f4:   e59dc000    ldr ip, [sp]
2860     23f8:   1731f21c    udivne  r1, ip, r2
2861     23fc:   1060c291    mlsne   r0, r1, r2, ip
2862     2400:   188e0009    stmne   lr, {r0, r3}
2863     2404:   e730f21c    udiv    r0, ip, r2
2864     2408:   ee800b90    vdup.32 d16, r0
2865     240c:   ea000014    b   2464 <__udivmoddi4+0x98>
2866     2410:   e35e0000    cmp lr, #0
2867     2414:   11cd00d0    ldrdne  r0, [sp]
2868     2418:   13a01000    movne   r1, #0
2869     241c:   11ce00f0    strdne  r0, [lr]
2870     2420:   f2c00010    vmov.i32    d16, #0 ; 0x00000000
2871     2424:   ea00000f    b   2468 <__udivmoddi4+0x9c>
2872     2428:   e3520000    cmp r2, #0
2873     242c:   1a00002c    bne 24e4 <__udivmoddi4+0x118>
2874     2430:   e3530000    cmp r3, #0
2875     2434:   1a000000    bne 243c <__udivmoddi4+0x70>
2876     2438:   e7f000f0    udf #0
2877     243c:   e59d7000    ldr r7, [sp]
2878     2440:   e3570000    cmp r7, #0
2879     2444:   e1a01007    mov r1, r7
2880     2448:   1a000008    bne 2470 <__udivmoddi4+0xa4>
2881     244c:   e73cf316    udiv    ip, r6, r3
2882     2450:   e35e0000    cmp lr, #0
2883     2454:   ee80cb90    vdup.32 d16, ip
2884     2458:   1066639c    mlsne   r6, ip, r3, r6
2885     245c:   11a03007    movne   r3, r7
2886     2460:   188e0048    stmne   lr, {r3, r6}
2887     2464:   f3e000b0    vshr.u64    d16, d16, #32
2888     2468:   edcd0b00    vstr    d16, [sp]
2889     246c:   ea000033    b   2540 <__udivmoddi4+0x174>
2890     2470:   e243c001    sub ip, r3, #1
2891     2474:   e11c0003    tst ip, r3
2892     2478:   1a000007    bne 249c <__udivmoddi4+0xd0>
2893     247c:   e35e0000    cmp lr, #0
2894     2480:   e6ff0f33    rbit    r0, r3
2895     2484:   159d1000    ldrne   r1, [sp]
2896     2488:   100cc006    andne   ip, ip, r6
2897     248c:   e16f0f10    clz r0, r0
2898     2490:   e1a00036    lsr r0, r6, r0
2899     2494:   188e1002    stmne   lr, {r1, ip}
2900     2498:   eaffffda    b   2408 <__udivmoddi4+0x3c>
2901     249c:   e16fcf13    clz ip, r3
2902     24a0:   e16f4f16    clz r4, r6
2903     24a4:   e04cc004    sub ip, ip, r4
2904     24a8:   e35c001e    cmp ip, #30
2905     24ac:   9a000003    bls 24c0 <__udivmoddi4+0xf4>
2906     24b0:   e35e0000    cmp lr, #0
2907     24b4:   11cd20d0    ldrdne  r2, [sp]
2908     24b8:   11ce20f0    strdne  r2, [lr]
2909     24bc:   eaffffd7    b   2420 <__udivmoddi4+0x54>
2910     24c0:   e28cc001    add ip, ip, #1
2911     24c4:   e58d2008    str r2, [sp, #8]
2912     24c8:   e26c1020    rsb r1, ip, #32
2913     24cc:   e1a0bc36    lsr fp, r6, ip
2914     24d0:   e1a00117    lsl r0, r7, r1
2915     24d4:   e58d000c    str r0, [sp, #12]
2916     24d8:   e1a00c37    lsr r0, r7, ip
2917     24dc:   e1800116    orr r0, r0, r6, lsl r1
2918     24e0:   ea00002b    b   2594 <__udivmoddi4+0x1c8>
2919     24e4:   e3530000    cmp r3, #0
2920     24e8:   1a000059    bne 2654 <__udivmoddi4+0x288>
2921     24ec:   e242c001    sub ip, r2, #1
2922     24f0:   e01c4002    ands    r4, ip, r2
2923     24f4:   1a000014    bne 254c <__udivmoddi4+0x180>
2924     24f8:   e35e0000    cmp lr, #0
2925     24fc:   159d1000    ldrne   r1, [sp]
2926     2500:   158e4004    strne   r4, [lr, #4]
2927     2504:   100cc001    andne   ip, ip, r1
2928     2508:   158ec000    strne   ip, [lr]
2929     250c:   e3520001    cmp r2, #1
2930     2510:   0a00000a    beq 2540 <__udivmoddi4+0x174>
2931     2514:   e6ff2f32    rbit    r2, r2
2932     2518:   e59d1000    ldr r1, [sp]
2933     251c:   e16f2f12    clz r2, r2
2934     2520:   e1a03236    lsr r3, r6, r2
2935     2524:   e58d300c    str r3, [sp, #12]
2936     2528:   e2623020    rsb r3, r2, #32
2937     252c:   e1a02231    lsr r2, r1, r2
2938     2530:   e1823316    orr r3, r2, r6, lsl r3
2939     2534:   e58d3008    str r3, [sp, #8]
2940     2538:   e1cd20d8    ldrd    r2, [sp, #8]
2941     253c:   e1cd20f0    strd    r2, [sp]
2942     2540:   e1cd00d0    ldrd    r0, [sp]
2943     2544:   e28dd014    add sp, sp, #20
2944     2548:   e8bd8ff0    pop {r4, r5, r6, r7, r8, r9, sl, fp, pc}
2945     254c:   e16fcf16    clz ip, r6
2946     2550:   e16f7f12    clz r7, r2
2947     2554:   e59d8000    ldr r8, [sp]
2948     2558:   e047700c    sub r7, r7, ip
2949     255c:   e287c021    add ip, r7, #33 ; 0x21
2950     2560:   e35c0020    cmp ip, #32
2951     2564:   01a01008    moveq   r1, r8
2952     2568:   058d3008    streq   r3, [sp, #8]
2953     256c:   0a000043    beq 2680 <__udivmoddi4+0x2b4>
2954     2570:   e35c001f    cmp ip, #31
2955     2574:   8a00002c    bhi 262c <__udivmoddi4+0x260>
2956     2578:   e26c0020    rsb r0, ip, #32
2957     257c:   e1a0bc36    lsr fp, r6, ip
2958     2580:   e58d3008    str r3, [sp, #8]
2959     2584:   e1a01018    lsl r1, r8, r0
2960     2588:   e1a08c38    lsr r8, r8, ip
2961     258c:   e1880016    orr r0, r8, r6, lsl r0
2962     2590:   e58d100c    str r1, [sp, #12]
2963     2594:   e59d7008    ldr r7, [sp, #8]
2964     2598:   e3a0a000    mov sl, #0
2965     259c:   e59d600c    ldr r6, [sp, #12]
2966     25a0:   e1a01fa0    lsr r1, r0, #31
2967     25a4:   e1a08fa7    lsr r8, r7, #31
2968     25a8:   e1a09001    mov r9, r1
2969     25ac:   e1a01fa6    lsr r1, r6, #31
2970     25b0:   e189508b    orr r5, r9, fp, lsl #1
2971     25b4:   e1814080    orr r4, r1, r0, lsl #1
2972     25b8:   e1886086    orr r6, r8, r6, lsl #1
2973     25bc:   e1e08004    mvn r8, r4
2974     25c0:   e0980002    adds    r0, r8, r2
2975     25c4:   e1e09005    mvn r9, r5
2976     25c8:   e0a91003    adc r1, r9, r3
2977     25cc:   e18a7087    orr r7, sl, r7, lsl #1
2978     25d0:   e1a08fc1    asr r8, r1, #31
2979     25d4:   e1a09fc1    asr r9, r1, #31
2980     25d8:   e0080002    and r0, r8, r2
2981     25dc:   e0091003    and r1, r9, r3
2982     25e0:   e208a001    and sl, r8, #1
2983     25e4:   e0544000    subs    r4, r4, r0
2984     25e8:   e0c55001    sbc r5, r5, r1
2985     25ec:   e25cc001    subs    ip, ip, #1
2986     25f0:   e1a08004    mov r8, r4
2987     25f4:   e1a09005    mov r9, r5
2988     25f8:   e1a00004    mov r0, r4
2989     25fc:   e1a0b005    mov fp, r5
2990     2600:   1affffe6    bne 25a0 <__udivmoddi4+0x1d4>
2991     2604:   e58d7008    str r7, [sp, #8]
2992     2608:   e58d600c    str r6, [sp, #12]
2993     260c:   e1cd40d8    ldrd    r4, [sp, #8]
2994     2610:   e0944004    adds    r4, r4, r4
2995     2614:   e0a55005    adc r5, r5, r5
2996     2618:   e35e0000    cmp lr, #0
2997     261c:   e184400a    orr r4, r4, sl
2998     2620:   11ce80f0    strdne  r8, [lr]
2999     2624:   e1cd40f0    strd    r4, [sp]
3000     2628:   eaffffc4    b   2540 <__udivmoddi4+0x174>
3001     262c:   e26c1040    rsb r1, ip, #64 ; 0x40
3002     2630:   e1a0b003    mov fp, r3
3003     2634:   e1a00118    lsl r0, r8, r1
3004     2638:   e58d0008    str r0, [sp, #8]
3005     263c:   e2870001    add r0, r7, #1
3006     2640:   e1a08038    lsr r8, r8, r0
3007     2644:   e1a00036    lsr r0, r6, r0
3008     2648:   e1881116    orr r1, r8, r6, lsl r1
3009     264c:   e58d100c    str r1, [sp, #12]
3010     2650:   eaffffcf    b   2594 <__udivmoddi4+0x1c8>
3011     2654:   e16fcf13    clz ip, r3
3012     2658:   e16f4f16    clz r4, r6
3013     265c:   e04cc004    sub ip, ip, r4
3014     2660:   e35c001f    cmp ip, #31
3015     2664:   8affff91    bhi 24b0 <__udivmoddi4+0xe4>
3016     2668:   f2c00010    vmov.i32    d16, #0 ; 0x00000000
3017     266c:   e28cc001    add ip, ip, #1
3018     2670:   e35c0020    cmp ip, #32
3019     2674:   edcd0b02    vstr    d16, [sp, #8]
3020     2678:   1a000004    bne 2690 <__udivmoddi4+0x2c4>
3021     267c:   e59d1000    ldr r1, [sp]
3022     2680:   e3a0b000    mov fp, #0
3023     2684:   e58d100c    str r1, [sp, #12]
3024     2688:   e59d0004    ldr r0, [sp, #4]
3025     268c:   eaffffc0    b   2594 <__udivmoddi4+0x1c8>
3026     2690:   e59d0000    ldr r0, [sp]
3027     2694:   e26c1020    rsb r1, ip, #32
3028     2698:   e1a0bc36    lsr fp, r6, ip
3029     269c:   e1a00110    lsl r0, r0, r1
3030     26a0:   e58d000c    str r0, [sp, #12]
3031     26a4:   e59d0000    ldr r0, [sp]
3032     26a8:   e1a00c30    lsr r0, r0, ip
3033     26ac:   eaffff8a    b   24dc <__udivmoddi4+0x110>
3034 
3035 000026b0 <__ctzdi2>:
3036     26b0:   e16f3f10    clz r3, r0
3037     26b4:   e1a032a3    lsr r3, r3, #5
3038     26b8:   e2632000    rsb r2, r3, #0
3039     26bc:   e2433001    sub r3, r3, #1
3040     26c0:   e0033000    and r3, r3, r0
3041     26c4:   e0011002    and r1, r1, r2
3042     26c8:   e1833001    orr r3, r3, r1
3043     26cc:   e2020020    and r0, r2, #32
3044     26d0:   e6ff3f33    rbit    r3, r3
3045     26d4:   e16f3f13    clz r3, r3
3046     26d8:   e0800003    add r0, r0, r3
3047     26dc:   e12fff1e    bx  lr
3048 
3049 000026e0 <__assert>:
3050     26e0:   e1a03002    mov r3, r2
3051     26e4:   e92d4010    push    {r4, lr}
3052     26e8:   e1a02001    mov r2, r1
3053     26ec:   e1a01000    mov r1, r0
3054     26f0:   e59f0008    ldr r0, [pc, #8]    ; 2700 <__assert+0x20>
3055     26f4:   ebffff1b    bl  2368 <tf_printf>
3056     26f8:   eb0005b1    bl  3dc4 <console_flush>
3057     26fc:   eb00059b    bl  3d70 <plat_panic_handler>
3058     2700:   00004f0a    .word   0x00004f0a
3059 
3060 00002704 <memcmp>:
3061     2704:   e0802002    add r2, r0, r2
3062     2708:   e1500002    cmp r0, r2
3063     270c:   1a000001    bne 2718 <memcmp+0x14>
3064     2710:   e3a00000    mov r0, #0
3065     2714:   e12fff1e    bx  lr
3066     2718:   e4d03001    ldrb    r3, [r0], #1
3067     271c:   e4d1c001    ldrb    ip, [r1], #1
3068     2720:   e153000c    cmp r3, ip
3069     2724:   0afffff7    beq 2708 <memcmp+0x4>
3070     2728:   e043000c    sub r0, r3, ip
3071     272c:   e12fff1e    bx  lr
3072 
3073 00002730 <memcpy>:
3074     2730:   e2403001    sub r3, r0, #1
3075     2734:   e0812002    add r2, r1, r2
3076     2738:   e1510002    cmp r1, r2
3077     273c:   012fff1e    bxeq    lr
3078     2740:   e4d1c001    ldrb    ip, [r1], #1
3079     2744:   e5e3c001    strb    ip, [r3, #1]!
3080     2748:   eafffffa    b   2738 <memcpy+0x8>
3081 
3082 0000274c <memmove>:
3083     274c:   e92d4010    push    {r4, lr}
3084     2750:   e0404001    sub r4, r0, r1
3085     2754:   e1540002    cmp r4, r2
3086     2758:   3a000001    bcc 2764 <memmove+0x18>
3087     275c:   e8bd4010    pop {r4, lr}
3088     2760:   eafffff2    b   2730 <memcpy>
3089     2764:   e1a0e000    mov lr, r0
3090     2768:   e081c002    add ip, r1, r2
3091     276c:   e0803002    add r3, r0, r2
3092     2770:   e15e0003    cmp lr, r3
3093     2774:   1a000001    bne 2780 <memmove+0x34>
3094     2778:   e1a0000e    mov r0, lr
3095     277c:   e8bd8010    pop {r4, pc}
3096     2780:   e57c2001    ldrb    r2, [ip, #-1]!
3097     2784:   e5632001    strb    r2, [r3, #-1]!
3098     2788:   eafffff8    b   2770 <memmove+0x24>
3099 
3100 0000278c <putchar>:
3101     278c:   e92d4010    push    {r4, lr}
3102     2790:   e1a04000    mov r4, r0
3103     2794:   e6ef0070    uxtb    r0, r0
3104     2798:   eb000585    bl  3db4 <console_putc>
3105     279c:   e3500000    cmp r0, #0
3106     27a0:   a1a00004    movge   r0, r4
3107     27a4:   b3e00000    mvnlt   r0, #0
3108     27a8:   e8bd8010    pop {r4, pc}
3109 
3110 000027ac <strlen>:
3111     27ac:   e1a03000    mov r3, r0
3112     27b0:   e1a02003    mov r2, r3
3113     27b4:   e2833001    add r3, r3, #1
3114     27b8:   e5d21000    ldrb    r1, [r2]
3115     27bc:   e3510000    cmp r1, #0
3116     27c0:   1afffffa    bne 27b0 <strlen+0x4>
3117     27c4:   e0420000    sub r0, r2, r0
3118     27c8:   e12fff1e    bx  lr
3119 
3120 000027cc <qemu_configure_mmu_secure>:
3121     27cc:   e92d47f3    push    {r0, r1, r4, r5, r6, r7, r8, r9, sl, lr}
3122     27d0:   e1a0a003    mov sl, r3
3123     27d4:   e3a0300a    mov r3, #10
3124     27d8:   e3a04000    mov r4, #0
3125     27dc:   e59d6028    ldr r6, [sp, #40]   ; 0x28
3126     27e0:   e1a07002    mov r7, r2
3127     27e4:   e1a02000    mov r2, r0
3128     27e8:   e59d902c    ldr r9, [sp, #44]   ; 0x2c
3129     27ec:   e58d3000    str r3, [sp]
3130     27f0:   e1a03001    mov r3, r1
3131     27f4:   e1a01004    mov r1, r4
3132     27f8:   e59d5030    ldr r5, [sp, #48]   ; 0x30
3133     27fc:   e59d8034    ldr r8, [sp, #52]   ; 0x34
3134     2800:   eb000315    bl  345c <mmap_add_region>
3135     2804:   e3a03002    mov r3, #2
3136     2808:   e1a02007    mov r2, r7
3137     280c:   e1a00007    mov r0, r7
3138     2810:   e1a01004    mov r1, r4
3139     2814:   e58d3000    str r3, [sp]
3140     2818:   e04a3007    sub r3, sl, r7
3141     281c:   eb00030e    bl  345c <mmap_add_region>
3142     2820:   e3a03022    mov r3, #34 ; 0x22
3143     2824:   e1a02006    mov r2, r6
3144     2828:   e1a00006    mov r0, r6
3145     282c:   e1a01004    mov r1, r4
3146     2830:   e58d3000    str r3, [sp]
3147     2834:   e0493006    sub r3, r9, r6
3148     2838:   eb000307    bl  345c <mmap_add_region>
3149     283c:   e3a03008    mov r3, #8
3150     2840:   e1a02005    mov r2, r5
3151     2844:   e1a01004    mov r1, r4
3152     2848:   e1a00005    mov r0, r5
3153     284c:   e58d3000    str r3, [sp]
3154     2850:   e0483005    sub r3, r8, r5
3155     2854:   eb000300    bl  345c <mmap_add_region>
3156     2858:   e59f0014    ldr r0, [pc, #20]   ; 2874 <qemu_configure_mmu_secure+0xa8>
3157     285c:   eb000316    bl  34bc <mmap_add>
3158     2860:   eb0003c7    bl  3784 <init_xlat_tables>
3159     2864:   e1a00004    mov r0, r4
3160     2868:   e28dd008    add sp, sp, #8
3161     286c:   e8bd47f0    pop {r4, r5, r6, r7, r8, r9, sl, lr}
3162     2870:   ea0003c6    b   3790 <enable_mmu_secure>
3163     2874:   00004f20    .word   0x00004f20
3164 
3165 00002878 <xlat_arch_get_max_supported_pa>:
3166     2878:   e3e00000    mvn r0, #0
3167     287c:   e3a010ff    mov r1, #255    ; 0xff
3168     2880:   e12fff1e    bx  lr
3169 
3170 00002884 <is_mmu_enabled_ctx>:
3171     2884:   ee110f10    mrc 15, 0, r0, cr1, cr0, {0}
3172     2888:   e2000001    and r0, r0, #1
3173     288c:   e12fff1e    bx  lr
3174 
3175 00002890 <enable_mmu_arch>:
3176     2890:   e92d4070    push    {r4, r5, r6, lr}
3177     2894:   e1a05001    mov r5, r1
3178     2898:   e59d1010    ldr r1, [sp, #16]
3179     289c:   ee113f11    mrc 15, 0, r3, cr1, cr1, {0}
3180     28a0:   e3130001    tst r3, #1
3181     28a4:   159f20c0    ldrne   r2, [pc, #192]  ; 296c <enable_mmu_arch+0xdc>
3182     28a8:   13a01067    movne   r1, #103    ; 0x67
3183     28ac:   1a000004    bne 28c4 <enable_mmu_arch+0x34>
3184     28b0:   ee114f10    mrc 15, 0, r4, cr1, cr0, {0}
3185     28b4:   e2143001    ands    r3, r4, #1
3186     28b8:   0a000003    beq 28cc <enable_mmu_arch+0x3c>
3187     28bc:   e59f20ac    ldr r2, [pc, #172]  ; 2970 <enable_mmu_arch+0xe0>
3188     28c0:   e3a0106a    mov r1, #106    ; 0x6a
3189     28c4:   e59f00a8    ldr r0, [pc, #168]  ; 2974 <enable_mmu_arch+0xe4>
3190     28c8:   ebffff84    bl  26e0 <__assert>
3191     28cc:   e1a06000    mov r6, r0
3192     28d0:   ee083f17    mcr 15, 0, r3, cr8, cr7, {0}
3193     28d4:   e3710001    cmn r1, #1
3194     28d8:   0a000021    beq 2964 <enable_mmu_arch+0xd4>
3195     28dc:   e2810001    add r0, r1, #1
3196     28e0:   e3500402    cmp r0, #33554432   ; 0x2000000
3197     28e4:   3a000001    bcc 28f0 <enable_mmu_arch+0x60>
3198     28e8:   e0111000    ands    r1, r1, r0
3199     28ec:   0a000002    beq 28fc <enable_mmu_arch+0x6c>
3200     28f0:   e59f2080    ldr r2, [pc, #128]  ; 2978 <enable_mmu_arch+0xe8>
3201     28f4:   e3a0108b    mov r1, #139    ; 0x8b
3202     28f8:   eafffff1    b   28c4 <enable_mmu_arch+0x34>
3203     28fc:   ebffff6b    bl  26b0 <__ctzdi2>
3204     2900:   e2600020    rsb r0, r0, #32
3205     2904:   e3800102    orr r0, r0, #-2147483648    ; 0x80000000
3206     2908:   e3800502    orr r0, r0, #8388608    ; 0x800000
3207     290c:   e3160002    tst r6, #2
3208     2910:   e59f3064    ldr r3, [pc, #100]  ; 297c <enable_mmu_arch+0xec>
3209     2914:   03800c35    orreq   r0, r0, #13568  ; 0x3500
3210     2918:   ee0a3f12    mcr 15, 0, r3, cr10, cr2, {0}
3211     291c:   ee020f50    mcr 15, 0, r0, cr2, cr0, {2}
3212     2920:   e3a03000    mov r3, #0
3213     2924:   e1a02005    mov r2, r5
3214     2928:   ec432f02    mcrr    15, 0, r2, r3, cr2
3215     292c:   e3a02000    mov r2, #0
3216     2930:   e3a03000    mov r3, #0
3217     2934:   ec432f12    mcrr    15, 1, r2, r3, cr2
3218     2938:   f57ff04b    dsb ish
3219     293c:   f57ff06f    isb sy
3220     2940:   e3160001    tst r6, #1
3221     2944:   13c43004    bicne   r3, r4, #4
3222     2948:   03843702    orreq   r3, r4, #524288 ; 0x80000
3223     294c:   13833702    orrne   r3, r3, #524288 ; 0x80000
3224     2950:   03833005    orreq   r3, r3, #5
3225     2954:   13833001    orrne   r3, r3, #1
3226     2958:   ee013f10    mcr 15, 0, r3, cr1, cr0, {0}
3227     295c:   f57ff06f    isb sy
3228     2960:   e8bd8070    pop {r4, r5, r6, pc}
3229     2964:   e59f0014    ldr r0, [pc, #20]   ; 2980 <enable_mmu_arch+0xf0>
3230     2968:   eaffffe7    b   290c <enable_mmu_arch+0x7c>
3231     296c:   00004f98    .word   0x00004f98
3232     2970:   00004fd5    .word   0x00004fd5
3233     2974:   00004fa7    .word   0x00004fa7
3234     2978:   00004ff0    .word   0x00004ff0
3235     297c:   004404ff    .word   0x004404ff
3236     2980:   80800000    .word   0x80800000
3237 
3238 00002984 <xlat_desc_print.isra.1>:
3239     2984:   e92d4ff1    push    {r0, r4, r5, r6, r7, r8, r9, sl, fp, lr}
3240     2988:   e1a0a002    mov sl, r2
3241     298c:   e1a02122    lsr r2, r2, #2
3242     2990:   e1a06000    mov r6, r0
3243     2994:   e1822f03    orr r2, r2, r3, lsl #30
3244     2998:   e2122003    ands    r2, r2, #3
3245     299c:   059f0110    ldreq   r0, [pc, #272]  ; 2ab4 <xlat_desc_print.isra.1+0x130>
3246     29a0:   0a000009    beq 29cc <xlat_desc_print.isra.1+0x48>
3247     29a4:   e3520002    cmp r2, #2
3248     29a8:   059f0108    ldreq   r0, [pc, #264]  ; 2ab8 <xlat_desc_print.isra.1+0x134>
3249     29ac:   0a000006    beq 29cc <xlat_desc_print.isra.1+0x48>
3250     29b0:   e3520001    cmp r2, #1
3251     29b4:   059f0100    ldreq   r0, [pc, #256]  ; 2abc <xlat_desc_print.isra.1+0x138>
3252     29b8:   0a000003    beq 29cc <xlat_desc_print.isra.1+0x48>
3253     29bc:   e59f20fc    ldr r2, [pc, #252]  ; 2ac0 <xlat_desc_print.isra.1+0x13c>
3254     29c0:   e300140b    movw    r1, #1035   ; 0x40b
3255     29c4:   e59f00f8    ldr r0, [pc, #248]  ; 2ac4 <xlat_desc_print.isra.1+0x140>
3256     29c8:   ebffff44    bl  26e0 <__assert>
3257     29cc:   ebfffe65    bl  2368 <tf_printf>
3258     29d0:   e3560001    cmp r6, #1
3259     29d4:   e20a2080    and r2, sl, #128    ; 0x80
3260     29d8:   e3a03000    mov r3, #0
3261     29dc:   e20a8010    and r8, sl, #16
3262     29e0:   e3a09000    mov r9, #0
3263     29e4:   1a000012    bne 2a34 <xlat_desc_print.isra.1+0xb0>
3264     29e8:   e1923003    orrs    r3, r2, r3
3265     29ec:   e59f30d4    ldr r3, [pc, #212]  ; 2ac8 <xlat_desc_print.isra.1+0x144>
3266     29f0:   e59f00d4    ldr r0, [pc, #212]  ; 2acc <xlat_desc_print.isra.1+0x148>
3267     29f4:   01a00003    moveq   r0, r3
3268     29f8:   ebfffe5a    bl  2368 <tf_printf>
3269     29fc:   e1983009    orrs    r3, r8, r9
3270     2a00:   e59f30c8    ldr r3, [pc, #200]  ; 2ad0 <xlat_desc_print.isra.1+0x14c>
3271     2a04:   e59f00c8    ldr r0, [pc, #200]  ; 2ad4 <xlat_desc_print.isra.1+0x150>
3272     2a08:   01a00003    moveq   r0, r3
3273     2a0c:   e20a4020    and r4, sl, #32
3274     2a10:   e3a05000    mov r5, #0
3275     2a14:   ebfffe53    bl  2368 <tf_printf>
3276     2a18:   e1943005    orrs    r3, r4, r5
3277     2a1c:   e59f30b4    ldr r3, [pc, #180]  ; 2ad8 <xlat_desc_print.isra.1+0x154>
3278     2a20:   e59f00b4    ldr r0, [pc, #180]  ; 2adc <xlat_desc_print.isra.1+0x158>
3279     2a24:   01a00003    moveq   r0, r3
3280     2a28:   e28dd004    add sp, sp, #4
3281     2a2c:   e8bd4ff0    pop {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3282     2a30:   eafffe4c    b   2368 <tf_printf>
3283     2a34:   e1923003    orrs    r3, r2, r3
3284     2a38:   e59f3088    ldr r3, [pc, #136]  ; 2ac8 <xlat_desc_print.isra.1+0x144>
3285     2a3c:   e59f6088    ldr r6, [pc, #136]  ; 2acc <xlat_desc_print.isra.1+0x148>
3286     2a40:   e59f708c    ldr r7, [pc, #140]  ; 2ad4 <xlat_desc_print.isra.1+0x150>
3287     2a44:   01a06003    moveq   r6, r3
3288     2a48:   e1a00006    mov r0, r6
3289     2a4c:   ebfffe45    bl  2368 <tf_printf>
3290     2a50:   e59f0088    ldr r0, [pc, #136]  ; 2ae0 <xlat_desc_print.isra.1+0x15c>
3291     2a54:   ebfffe43    bl  2368 <tf_printf>
3292     2a58:   e20a2040    and r2, sl, #64 ; 0x40
3293     2a5c:   e3a03000    mov r3, #0
3294     2a60:   e59f007c    ldr r0, [pc, #124]  ; 2ae4 <xlat_desc_print.isra.1+0x160>
3295     2a64:   e1923003    orrs    r3, r2, r3
3296     2a68:   11a00006    movne   r0, r6
3297     2a6c:   e59f605c    ldr r6, [pc, #92]   ; 2ad0 <xlat_desc_print.isra.1+0x14c>
3298     2a70:   ebfffe3c    bl  2368 <tf_printf>
3299     2a74:   e59f006c    ldr r0, [pc, #108]  ; 2ae8 <xlat_desc_print.isra.1+0x164>
3300     2a78:   ebfffe3a    bl  2368 <tf_printf>
3301     2a7c:   e3a03000    mov r3, #0
3302     2a80:   e20a2008    and r2, sl, #8
3303     2a84:   e1923003    orrs    r3, r2, r3
3304     2a88:   11a00007    movne   r0, r7
3305     2a8c:   01a00006    moveq   r0, r6
3306     2a90:   ebfffe34    bl  2368 <tf_printf>
3307     2a94:   e59f0044    ldr r0, [pc, #68]   ; 2ae0 <xlat_desc_print.isra.1+0x15c>
3308     2a98:   ebfffe32    bl  2368 <tf_printf>
3309     2a9c:   e1983009    orrs    r3, r8, r9
3310     2aa0:   11a00007    movne   r0, r7
3311     2aa4:   01a00006    moveq   r0, r6
3312     2aa8:   ebfffe2e    bl  2368 <tf_printf>
3313     2aac:   e59f0034    ldr r0, [pc, #52]   ; 2ae8 <xlat_desc_print.isra.1+0x164>
3314     2ab0:   eaffffd5    b   2a0c <xlat_desc_print.isra.1+0x88>
3315     2ab4:   000054e4    .word   0x000054e4
3316     2ab8:   000054e8    .word   0x000054e8
3317     2abc:   0000550f    .word   0x0000550f
3318     2ac0:   000054eb    .word   0x000054eb
3319     2ac4:   000050ff    .word   0x000050ff
3320     2ac8:   000054c5    .word   0x000054c5
3321     2acc:   000054c1    .word   0x000054c1
3322     2ad0:   000054d7    .word   0x000054d7
3323     2ad4:   000054d3    .word   0x000054d3
3324     2ad8:   000054e1    .word   0x000054e1
3325     2adc:   000054dd    .word   0x000054dd
3326     2ae0:   00005513    .word   0x00005513
3327     2ae4:   000054c9    .word   0x000054c9
3328     2ae8:   0000551a    .word   0x0000551a
3329 
3330 00002aec <xlat_tables_print_internal>:
3331     2aec:   e92d4ff0    push    {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3332     2af0:   e24dd02c    sub sp, sp, #44 ; 0x2c
3333     2af4:   e59d5050    ldr r5, [sp, #80]   ; 0x50
3334     2af8:   e58d0020    str r0, [sp, #32]
3335     2afc:   e58d3024    str r3, [sp, #36]   ; 0x24
3336     2b00:   e3550003    cmp r5, #3
3337     2b04:   9a000003    bls 2b18 <xlat_tables_print_internal+0x2c>
3338     2b08:   e59f2184    ldr r2, [pc, #388]  ; 2c94 <xlat_tables_print_internal+0x1a8>
3339     2b0c:   e300144f    movw    r1, #1103   ; 0x44f
3340     2b10:   e59f0180    ldr r0, [pc, #384]  ; 2c98 <xlat_tables_print_internal+0x1ac>
3341     2b14:   ebfffef1    bl  26e0 <__assert>
3342     2b18:   e3e06008    mvn r6, #8
3343     2b1c:   e3a04000    mov r4, #0
3344     2b20:   e1a07001    mov r7, r1
3345     2b24:   e1a0a004    mov sl, r4
3346     2b28:   e0060596    mul r6, r6, r5
3347     2b2c:   e2863027    add r3, r6, #39 ; 0x27
3348     2b30:   e3a06001    mov r6, #1
3349     2b34:   e1a06316    lsl r6, r6, r3
3350     2b38:   e2423008    sub r3, r2, #8
3351     2b3c:   e58d301c    str r3, [sp, #28]
3352     2b40:   e59d3024    ldr r3, [sp, #36]   ; 0x24
3353     2b44:   e15a0003    cmp sl, r3
3354     2b48:   ba000008    blt 2b70 <xlat_tables_print_internal+0x84>
3355     2b4c:   e3540001    cmp r4, #1
3356     2b50:   da00004d    ble 2c8c <xlat_tables_print_internal+0x1a0>
3357     2b54:   e59f3140    ldr r3, [pc, #320]  ; 2c9c <xlat_tables_print_internal+0x1b0>
3358     2b58:   e2442001    sub r2, r4, #1
3359     2b5c:   e59f013c    ldr r0, [pc, #316]  ; 2ca0 <xlat_tables_print_internal+0x1b4>
3360     2b60:   e7931105    ldr r1, [r3, r5, lsl #2]
3361     2b64:   e28dd02c    add sp, sp, #44 ; 0x2c
3362     2b68:   e8bd4ff0    pop {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3363     2b6c:   eafffdfd    b   2368 <tf_printf>
3364     2b70:   e59d301c    ldr r3, [sp, #28]
3365     2b74:   e1e380d8    ldrd    r8, [r3, #8]!
3366     2b78:   e2082003    and r2, r8, #3
3367     2b7c:   e58d301c    str r3, [sp, #28]
3368     2b80:   e3a03000    mov r3, #0
3369     2b84:   e1cd21f0    strd    r2, [sp, #16]
3370     2b88:   e1923003    orrs    r3, r2, r3
3371     2b8c:   1a00000b    bne 2bc0 <xlat_tables_print_internal+0xd4>
3372     2b90:   e3540000    cmp r4, #0
3373     2b94:   1a000005    bne 2bb0 <xlat_tables_print_internal+0xc4>
3374     2b98:   e59f10fc    ldr r1, [pc, #252]  ; 2c9c <xlat_tables_print_internal+0x1b0>
3375     2b9c:   e1a03006    mov r3, r6
3376     2ba0:   e1a02007    mov r2, r7
3377     2ba4:   e59f00f8    ldr r0, [pc, #248]  ; 2ca4 <xlat_tables_print_internal+0x1b8>
3378     2ba8:   e7911105    ldr r1, [r1, r5, lsl #2]
3379     2bac:   ebfffded    bl  2368 <tf_printf>
3380     2bb0:   e2844001    add r4, r4, #1
3381     2bb4:   e28aa001    add sl, sl, #1
3382     2bb8:   e0877006    add r7, r7, r6
3383     2bbc:   eaffffdf    b   2b40 <xlat_tables_print_internal+0x54>
3384     2bc0:   e59f30d4    ldr r3, [pc, #212]  ; 2c9c <xlat_tables_print_internal+0x1b0>
3385     2bc4:   e3540001    cmp r4, #1
3386     2bc8:   e793b105    ldr fp, [r3, r5, lsl #2]
3387     2bcc:   da000003    ble 2be0 <xlat_tables_print_internal+0xf4>
3388     2bd0:   e2442001    sub r2, r4, #1
3389     2bd4:   e1a0100b    mov r1, fp
3390     2bd8:   e59f00c0    ldr r0, [pc, #192]  ; 2ca0 <xlat_tables_print_internal+0x1b4>
3391     2bdc:   ebfffde1    bl  2368 <tf_printf>
3392     2be0:   e1cd21d0    ldrd    r2, [sp, #16]
3393     2be4:   e3530000    cmp r3, #0
3394     2be8:   03520003    cmpeq   r2, #3
3395     2bec:   03a04001    moveq   r4, #1
3396     2bf0:   13a04000    movne   r4, #0
3397     2bf4:   e3550003    cmp r5, #3
3398     2bf8:   03a04000    moveq   r4, #0
3399     2bfc:   12044001    andne   r4, r4, #1
3400     2c00:   e3540000    cmp r4, #0
3401     2c04:   0a00000e    beq 2c44 <xlat_tables_print_internal+0x158>
3402     2c08:   e1a03006    mov r3, r6
3403     2c0c:   e1a02007    mov r2, r7
3404     2c10:   e59f008c    ldr r0, [pc, #140]  ; 2ca4 <xlat_tables_print_internal+0x1b8>
3405     2c14:   e1a0100b    mov r1, fp
3406     2c18:   e3a04000    mov r4, #0
3407     2c1c:   ebfffdd1    bl  2368 <tf_printf>
3408     2c20:   e2853001    add r3, r5, #1
3409     2c24:   e3c82eff    bic r2, r8, #4080   ; 0xff0
3410     2c28:   e59d0020    ldr r0, [sp, #32]
3411     2c2c:   e3c2200f    bic r2, r2, #15
3412     2c30:   e1a01007    mov r1, r7
3413     2c34:   e58d3000    str r3, [sp]
3414     2c38:   e3a03c02    mov r3, #512    ; 0x200
3415     2c3c:   ebffffaa    bl  2aec <xlat_tables_print_internal>
3416     2c40:   eaffffdb    b   2bb4 <xlat_tables_print_internal+0xc8>
3417     2c44:   e59f205c    ldr r2, [pc, #92]   ; 2ca8 <xlat_tables_print_internal+0x1bc>
3418     2c48:   e30f3fff    movw    r3, #65535  ; 0xffff
3419     2c4c:   e1a0100b    mov r1, fp
3420     2c50:   e0033009    and r3, r3, r9
3421     2c54:   e59f0050    ldr r0, [pc, #80]   ; 2cac <xlat_tables_print_internal+0x1c0>
3422     2c58:   e58d6008    str r6, [sp, #8]
3423     2c5c:   e0022008    and r2, r2, r8
3424     2c60:   e1cd20f0    strd    r2, [sp]
3425     2c64:   e1a02007    mov r2, r7
3426     2c68:   ebfffdbe    bl  2368 <tf_printf>
3427     2c6c:   e59d1020    ldr r1, [sp, #32]
3428     2c70:   e1a02008    mov r2, r8
3429     2c74:   e1a03009    mov r3, r9
3430     2c78:   e591003c    ldr r0, [r1, #60]   ; 0x3c
3431     2c7c:   ebffff40    bl  2984 <xlat_desc_print.isra.1>
3432     2c80:   e59f0028    ldr r0, [pc, #40]   ; 2cb0 <xlat_tables_print_internal+0x1c4>
3433     2c84:   ebfffdb7    bl  2368 <tf_printf>
3434     2c88:   eaffffc9    b   2bb4 <xlat_tables_print_internal+0xc8>
3435     2c8c:   e28dd02c    add sp, sp, #44 ; 0x2c
3436     2c90:   e8bd8ff0    pop {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3437     2c94:   0000553d    .word   0x0000553d
3438     2c98:   000050ff    .word   0x000050ff
3439     2c9c:   000053b0    .word   0x000053b0
3440     2ca0:   000056f9    .word   0x000056f9
3441     2ca4:   000056e5    .word   0x000056e5
3442     2ca8:   fffff000    .word   0xfffff000
3443     2cac:   0000571d    .word   0x0000571d
3444     2cb0:   00004aba    .word   0x00004aba
3445 
3446 00002cb4 <xlat_desc.isra.2>:
3447     2cb4:   e92d4ff1    push    {r0, r4, r5, r6, r7, r8, r9, sl, fp, lr}
3448     2cb8:   e3e0c008    mvn ip, #8
3449     2cbc:   e59d4028    ldr r4, [sp, #40]   ; 0x28
3450     2cc0:   e3e0e000    mvn lr, #0
3451     2cc4:   e3a07000    mov r7, #0
3452     2cc8:   e00c049c    mul ip, ip, r4
3453     2ccc:   e28cc027    add ip, ip, #39 ; 0x27
3454     2cd0:   e1e0cc1e    mvn ip, lr, lsl ip
3455     2cd4:   e002600c    and r6, r2, ip
3456     2cd8:   e196c007    orrs    ip, r6, r7
3457     2cdc:   159f20d0    ldrne   r2, [pc, #208]  ; 2db4 <xlat_desc.isra.2+0x100>
3458     2ce0:   13a0107d    movne   r1, #125    ; 0x7d
3459     2ce4:   1a000030    bne 2dac <xlat_desc.isra.2+0xf8>
3460     2ce8:   ee801b90    vdup.32 d16, r1
3461     2cec:   e3540003    cmp r4, #3
3462     2cf0:   13a06001    movne   r6, #1
3463     2cf4:   03a06003    moveq   r6, #3
3464     2cf8:   e5900000    ldr r0, [r0]
3465     2cfc:   e3a07000    mov r7, #0
3466     2d00:   e3a0b000    mov fp, #0
3467     2d04:   f3e000b0    vshr.u64    d16, d16, #32
3468     2d08:   f27008a0    vadd.i64    d16, d16, d16
3469     2d0c:   e3110008    tst r1, #8
3470     2d10:   03a0a080    moveq   sl, #128    ; 0x80
3471     2d14:   13a0a000    movne   sl, #0
3472     2d18:   e3500000    cmp r0, #0
3473     2d1c:   ec598b30    vmov    r8, r9, d16
3474     2d20:   e3a09000    mov r9, #0
3475     2d24:   e1835009    orr r5, r3, r9
3476     2d28:   e1853007    orr r3, r5, r7
3477     2d2c:   e18b5003    orr r5, fp, r3
3478     2d30:   e1a03005    mov r3, r5
3479     2d34:   e2088020    and r8, r8, #32
3480     2d38:   e1824008    orr r4, r2, r8
3481     2d3c:   e1842006    orr r2, r4, r6
3482     2d40:   e18a4002    orr r4, sl, r2
3483     2d44:   e3842b01    orr r2, r4, #1024   ; 0x400
3484     2d48:   1a000013    bne 2d9c <xlat_desc.isra.2+0xe8>
3485     2d4c:   e3110040    tst r1, #64 ; 0x40
3486     2d50:   13842d11    orrne   r2, r4, #1088   ; 0x440
3487     2d54:   11a03005    movne   r3, r5
3488     2d58:   e2110007    ands    r0, r1, #7
3489     2d5c:   03820f81    orreq   r0, r2, #516    ; 0x204
3490     2d60:   03831501    orreq   r1, r3, #4194304    ; 0x400000
3491     2d64:   0a00000a    beq 2d94 <xlat_desc.isra.2+0xe0>
3492     2d68:   e3110028    tst r1, #40 ; 0x28
3493     2d6c:   13833501    orrne   r3, r3, #4194304    ; 0x400000
3494     2d70:   e3500002    cmp r0, #2
3495     2d74:   03820c03    orreq   r0, r2, #768    ; 0x300
3496     2d78:   0a000004    beq 2d90 <xlat_desc.isra.2+0xdc>
3497     2d7c:   e3500001    cmp r0, #1
3498     2d80:   159f2030    ldrne   r2, [pc, #48]   ; 2db8 <xlat_desc.isra.2+0x104>
3499     2d84:   13a010cf    movne   r1, #207    ; 0xcf
3500     2d88:   1a000007    bne 2dac <xlat_desc.isra.2+0xf8>
3501     2d8c:   e3820f82    orr r0, r2, #520    ; 0x208
3502     2d90:   e1a01003    mov r1, r3
3503     2d94:   e28dd004    add sp, sp, #4
3504     2d98:   e8bd8ff0    pop {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3505     2d9c:   e3500001    cmp r0, #1
3506     2da0:   0affffec    beq 2d58 <xlat_desc.isra.2+0xa4>
3507     2da4:   e59f2010    ldr r2, [pc, #16]   ; 2dbc <xlat_desc.isra.2+0x108>
3508     2da8:   e3a0109d    mov r1, #157    ; 0x9d
3509     2dac:   e59f000c    ldr r0, [pc, #12]   ; 2dc0 <xlat_desc.isra.2+0x10c>
3510     2db0:   ebfffe4a    bl  26e0 <__assert>
3511     2db4:   0000547c    .word   0x0000547c
3512     2db8:   000054a4    .word   0x000054a4
3513     2dbc:   0000520a    .word   0x0000520a
3514     2dc0:   000050ff    .word   0x000050ff
3515 
3516 00002dc4 <xlat_tables_map_region>:
3517     2dc4:   e92d4ff0    push    {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3518     2dc8:   e24dd034    sub sp, sp, #52 ; 0x34
3519     2dcc:   e5904034    ldr r4, [r0, #52]   ; 0x34
3520     2dd0:   e59d505c    ldr r5, [sp, #92]   ; 0x5c
3521     2dd4:   e3550003    cmp r5, #3
3522     2dd8:   91540005    cmpls   r4, r5
3523     2ddc:   83a04001    movhi   r4, #1
3524     2de0:   93a04000    movls   r4, #0
3525     2de4:   859f2290    ldrhi   r2, [pc, #656]  ; 307c <xlat_tables_map_region+0x2b8>
3526     2de8:   83001209    movwhi  r1, #521    ; 0x209
3527     2dec:   8a000018    bhi 2e54 <xlat_tables_map_region+0x90>
3528     2df0:   e1a07000    mov r7, r0
3529     2df4:   e1a06001    mov r6, r1
3530     2df8:   e1c100d8    ldrd    r0, [r1, #8]
3531     2dfc:   e0801001    add r1, r0, r1
3532     2e00:   e1500002    cmp r0, r2
3533     2e04:   e2411001    sub r1, r1, #1
3534     2e08:   e58d1024    str r1, [sp, #36]   ; 0x24
3535     2e0c:   e3e01008    mvn r1, #8
3536     2e10:   e0010591    mul r1, r1, r5
3537     2e14:   e2811027    add r1, r1, #39 ; 0x27
3538     2e18:   e58d1010    str r1, [sp, #16]
3539     2e1c:   e3a01001    mov r1, #1
3540     2e20:   e59dc010    ldr ip, [sp, #16]
3541     2e24:   e1a01c11    lsl r1, r1, ip
3542     2e28:   e58d1014    str r1, [sp, #20]
3543     2e2c:   9a000064    bls 2fc4 <xlat_tables_map_region+0x200>
3544     2e30:   e2611000    rsb r1, r1, #0
3545     2e34:   e0000001    and r0, r0, r1
3546     2e38:   e0402002    sub r2, r0, r2
3547     2e3c:   e1a04c32    lsr r4, r2, ip
3548     2e40:   e59d2058    ldr r2, [sp, #88]   ; 0x58
3549     2e44:   e1540002    cmp r4, r2
3550     2e48:   ba00005e    blt 2fc8 <xlat_tables_map_region+0x204>
3551     2e4c:   e59f222c    ldr r2, [pc, #556]  ; 3080 <xlat_tables_map_region+0x2bc>
3552     2e50:   e3a01f87    mov r1, #540    ; 0x21c
3553     2e54:   e59f0228    ldr r0, [pc, #552]  ; 3084 <xlat_tables_map_region+0x2c0>
3554     2e58:   ebfffe20    bl  26e0 <__assert>
3555     2e5c:   e3530003    cmp r3, #3
3556     2e60:   0a00004f    beq 2fa4 <xlat_tables_map_region+0x1e0>
3557     2e64:   e3530000    cmp r3, #0
3558     2e68:   1a000014    bne 2ec0 <xlat_tables_map_region+0xfc>
3559     2e6c:   e1cd22d8    ldrd    r2, [sp, #40]   ; 0x28
3560     2e70:   e003300b    and r3, r3, fp
3561     2e74:   e002200a    and r2, r2, sl
3562     2e78:   e1923003    orrs    r3, r2, r3
3563     2e7c:   13a03001    movne   r3, #1
3564     2e80:   03a03000    moveq   r3, #0
3565     2e84:   e3550000    cmp r5, #0
3566     2e88:   03a03001    moveq   r3, #1
3567     2e8c:   e3530000    cmp r3, #0
3568     2e90:   1a000003    bne 2ea4 <xlat_tables_map_region+0xe0>
3569     2e94:   e59d2010    ldr r2, [sp, #16]
3570     2e98:   e5963014    ldr r3, [r6, #20]
3571     2e9c:   e1b03233    lsrs    r3, r3, r2
3572     2ea0:   1a00001b    bne 2f14 <xlat_tables_map_region+0x150>
3573     2ea4:   e5973018    ldr r3, [r7, #24]
3574     2ea8:   e597201c    ldr r2, [r7, #28]
3575     2eac:   e1520003    cmp r2, r3
3576     2eb0:   3a000026    bcc 2f50 <xlat_tables_map_region+0x18c>
3577     2eb4:   e59f21cc    ldr r2, [pc, #460]  ; 3088 <xlat_tables_map_region+0x2c4>
3578     2eb8:   e3a0106c    mov r1, #108    ; 0x6c
3579     2ebc:   eaffffe4    b   2e54 <xlat_tables_map_region+0x90>
3580     2ec0:   e59d300c    ldr r3, [sp, #12]
3581     2ec4:   e3530001    cmp r3, #1
3582     2ec8:   0a000018    beq 2f30 <xlat_tables_map_region+0x16c>
3583     2ecc:   e59f21b8    ldr r2, [pc, #440]  ; 308c <xlat_tables_map_region+0x2c8>
3584     2ed0:   e3a01f72    mov r1, #456    ; 0x1c8
3585     2ed4:   eaffffde    b   2e54 <xlat_tables_map_region+0x90>
3586     2ed8:   e150000c    cmp r0, ip
3587     2edc:   81510009    cmphi   r1, r9
3588     2ee0:   8a000012    bhi 2f30 <xlat_tables_map_region+0x16c>
3589     2ee4:   e3550003    cmp r5, #3
3590     2ee8:   059f21a0    ldreq   r2, [pc, #416]  ; 3090 <xlat_tables_map_region+0x2cc>
3591     2eec:   030011dd    movweq  r1, #477    ; 0x1dd
3592     2ef0:   0affffd7    beq 2e54 <xlat_tables_map_region+0x90>
3593     2ef4:   e59d300c    ldr r3, [sp, #12]
3594     2ef8:   e3530000    cmp r3, #0
3595     2efc:   0affffe8    beq 2ea4 <xlat_tables_map_region+0xe0>
3596     2f00:   e3530003    cmp r3, #3
3597     2f04:   0a000026    beq 2fa4 <xlat_tables_map_region+0x1e0>
3598     2f08:   e59f2184    ldr r2, [pc, #388]  ; 3094 <xlat_tables_map_region+0x2d0>
3599     2f0c:   e3a01f7a    mov r1, #488    ; 0x1e8
3600     2f10:   eaffffcf    b   2e54 <xlat_tables_map_region+0x90>
3601     2f14:   e1a0200a    mov r2, sl
3602     2f18:   e1a0300b    mov r3, fp
3603     2f1c:   e58d5000    str r5, [sp]
3604     2f20:   e287003c    add r0, r7, #60 ; 0x3c
3605     2f24:   e5961010    ldr r1, [r6, #16]
3606     2f28:   ebffff61    bl  2cb4 <xlat_desc.isra.2>
3607     2f2c:   e1c800f0    strd    r0, [r8]
3608     2f30:   e59d0018    ldr r0, [sp, #24]
3609     2f34:   e2844001    add r4, r4, #1
3610     2f38:   e2888008    add r8, r8, #8
3611     2f3c:   e59d3024    ldr r3, [sp, #36]   ; 0x24
3612     2f40:   e1530000    cmp r3, r0
3613     2f44:   8a000027    bhi 2fe8 <xlat_tables_map_region+0x224>
3614     2f48:   e2400001    sub r0, r0, #1
3615     2f4c:   ea000012    b   2f9c <xlat_tables_map_region+0x1d8>
3616     2f50:   e5973014    ldr r3, [r7, #20]
3617     2f54:   e2821001    add r1, r2, #1
3618     2f58:   e587101c    str r1, [r7, #28]
3619     2f5c:   e0933602    adds    r3, r3, r2, lsl #12
3620     2f60:   0a00000d    beq 2f9c <xlat_tables_map_region+0x1d8>
3621     2f64:   e3832003    orr r2, r3, #3
3622     2f68:   e5882000    str r2, [r8]
3623     2f6c:   e3a02000    mov r2, #0
3624     2f70:   e5882004    str r2, [r8, #4]
3625     2f74:   e59d2020    ldr r2, [sp, #32]
3626     2f78:   e58d2004    str r2, [sp, #4]
3627     2f7c:   e3a02c02    mov r2, #512    ; 0x200
3628     2f80:   e58d2000    str r2, [sp]
3629     2f84:   e1a02000    mov r2, r0
3630     2f88:   e1a01006    mov r1, r6
3631     2f8c:   e1a00007    mov r0, r7
3632     2f90:   ebffff8b    bl  2dc4 <xlat_tables_map_region>
3633     2f94:   e1500009    cmp r0, r9
3634     2f98:   0affffe4    beq 2f30 <xlat_tables_map_region+0x16c>
3635     2f9c:   e28dd034    add sp, sp, #52 ; 0x34
3636     2fa0:   e8bd8ff0    pop {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3637     2fa4:   e59d301c    ldr r3, [sp, #28]
3638     2fa8:   e59d2020    ldr r2, [sp, #32]
3639     2fac:   e3c33eff    bic r3, r3, #4080   ; 0xff0
3640     2fb0:   e58d2004    str r2, [sp, #4]
3641     2fb4:   e3a02c02    mov r2, #512    ; 0x200
3642     2fb8:   e3c3300f    bic r3, r3, #15
3643     2fbc:   e58d2000    str r2, [sp]
3644     2fc0:   eaffffef    b   2f84 <xlat_tables_map_region+0x1c0>
3645     2fc4:   e1a00002    mov r0, r2
3646     2fc8:   e59d2014    ldr r2, [sp, #20]
3647     2fcc:   e0838184    add r8, r3, r4, lsl #3
3648     2fd0:   e2853001    add r3, r5, #1
3649     2fd4:   e58d3020    str r3, [sp, #32]
3650     2fd8:   e2422001    sub r2, r2, #1
3651     2fdc:   ee802b90    vdup.32 d16, r2
3652     2fe0:   f3e000b0    vshr.u64    d16, d16, #32
3653     2fe4:   edcd0b0a    vstr    d16, [sp, #40]  ; 0x28
3654     2fe8:   e59d3058    ldr r3, [sp, #88]   ; 0x58
3655     2fec:   e1540003    cmp r4, r3
3656     2ff0:   aaffffd4    bge 2f48 <xlat_tables_map_region+0x184>
3657     2ff4:   e5961008    ldr r1, [r6, #8]
3658     2ff8:   e596c00c    ldr ip, [r6, #12]
3659     2ffc:   e59de014    ldr lr, [sp, #20]
3660     3000:   e5983000    ldr r3, [r8]
3661     3004:   e081c00c    add ip, r1, ip
3662     3008:   e080e00e    add lr, r0, lr
3663     300c:   e24cc001    sub ip, ip, #1
3664     3010:   e24e9001    sub r9, lr, #1
3665     3014:   e15c0009    cmp ip, r9
3666     3018:   e58de018    str lr, [sp, #24]
3667     301c:   33a0e000    movcc   lr, #0
3668     3020:   23a0e001    movcs   lr, #1
3669     3024:   e58d301c    str r3, [sp, #28]
3670     3028:   e1510000    cmp r1, r0
3671     302c:   e2033003    and r3, r3, #3
3672     3030:   83a0e000    movhi   lr, #0
3673     3034:   e35e0000    cmp lr, #0
3674     3038:   e58d300c    str r3, [sp, #12]
3675     303c:   0affffa5    beq 2ed8 <xlat_tables_map_region+0x114>
3676     3040:   e1c6a0d0    ldrd    sl, [r6]
3677     3044:   e05a2001    subs    r2, sl, r1
3678     3048:   e2cb3000    sbc r3, fp, #0
3679     304c:   e092a000    adds    sl, r2, r0
3680     3050:   e2a3b000    adc fp, r3, #0
3681     3054:   e3550003    cmp r5, #3
3682     3058:   e59d300c    ldr r3, [sp, #12]
3683     305c:   1affff7e    bne 2e5c <xlat_tables_map_region+0x98>
3684     3060:   e3530003    cmp r3, #3
3685     3064:   0affffb1    beq 2f30 <xlat_tables_map_region+0x16c>
3686     3068:   e3530000    cmp r3, #0
3687     306c:   0affffa8    beq 2f14 <xlat_tables_map_region+0x150>
3688     3070:   e59f2020    ldr r2, [pc, #32]   ; 3098 <xlat_tables_map_region+0x2d4>
3689     3074:   e30011a1    movw    r1, #417    ; 0x1a1
3690     3078:   eaffff75    b   2e54 <xlat_tables_map_region+0x90>
3691     307c:   00005521    .word   0x00005521
3692     3080:   0000555b    .word   0x0000555b
3693     3084:   000050ff    .word   0x000050ff
3694     3088:   000055b1    .word   0x000055b1
3695     308c:   0000558f    .word   0x0000558f
3696     3090:   000055a7    .word   0x000055a7
3697     3094:   0000518e    .word   0x0000518e
3698     3098:   00005575    .word   0x00005575
3699 
3700 0000309c <print_mmap>:
3701     309c:   e92d4073    push    {r0, r1, r4, r5, r6, lr}
3702     30a0:   e1a04000    mov r4, r0
3703     30a4:   e59f5054    ldr r5, [pc, #84]   ; 3100 <print_mmap+0x64>
3704     30a8:   e59f6054    ldr r6, [pc, #84]   ; 3104 <print_mmap+0x68>
3705     30ac:   e59f0054    ldr r0, [pc, #84]   ; 3108 <print_mmap+0x6c>
3706     30b0:   ebfffcac    bl  2368 <tf_printf>
3707     30b4:   e2844018    add r4, r4, #24
3708     30b8:   e514300c    ldr r3, [r4, #-12]
3709     30bc:   e3530000    cmp r3, #0
3710     30c0:   1a000003    bne 30d4 <print_mmap+0x38>
3711     30c4:   e59f0040    ldr r0, [pc, #64]   ; 310c <print_mmap+0x70>
3712     30c8:   e28dd008    add sp, sp, #8
3713     30cc:   e8bd4070    pop {r4, r5, r6, lr}
3714     30d0:   eafffca4    b   2368 <tf_printf>
3715     30d4:   e5142008    ldr r2, [r4, #-8]
3716     30d8:   e1a00005    mov r0, r5
3717     30dc:   e58d3000    str r3, [sp]
3718     30e0:   e58d2004    str r2, [sp, #4]
3719     30e4:   e14421d8    ldrd    r2, [r4, #-24]  ; 0xffffffe8
3720     30e8:   e5141010    ldr r1, [r4, #-16]
3721     30ec:   ebfffc9d    bl  2368 <tf_printf>
3722     30f0:   e1a00006    mov r0, r6
3723     30f4:   e5141004    ldr r1, [r4, #-4]
3724     30f8:   ebfffc9a    bl  2368 <tf_printf>
3725     30fc:   eaffffec    b   30b4 <print_mmap+0x18>
3726     3100:   00005417    .word   0x00005417
3727     3104:   00005440    .word   0x00005440
3728     3108:   00005410    .word   0x00005410
3729     310c:   00004aba    .word   0x00004aba
3730 
3731 00003110 <mmap_add_region_ctx>:
3732     3110:   e591c00c    ldr ip, [r1, #12]
3733     3114:   e35c0000    cmp ip, #0
3734     3118:   012fff1e    bxeq    lr
3735     311c:   e5903038    ldr r3, [r0, #56]   ; 0x38
3736     3120:   e92d4ff0    push    {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3737     3124:   e1a04001    mov r4, r1
3738     3128:   e24dd02c    sub sp, sp, #44 ; 0x2c
3739     312c:   e1a05000    mov r5, r0
3740     3130:   e3530000    cmp r3, #0
3741     3134:   0a000003    beq 3148 <mmap_add_region_ctx+0x38>
3742     3138:   e59f2308    ldr r2, [pc, #776]  ; 3448 <mmap_add_region_ctx+0x338>
3743     313c:   e3a01fbb    mov r1, #748    ; 0x2ec
3744     3140:   e59f0304    ldr r0, [pc, #772]  ; 344c <mmap_add_region_ctx+0x33c>
3745     3144:   ebfffd65    bl  26e0 <__assert>
3746     3148:   e1c420d0    ldrd    r2, [r4]
3747     314c:   e3000fff    movw    r0, #4095   ; 0xfff
3748     3150:   e3a01000    mov r1, #0
3749     3154:   e1cd20f0    strd    r2, [sp]
3750     3158:   e5942014    ldr r2, [r4, #20]
3751     315c:   e1cd60d0    ldrd    r6, [sp]
3752     3160:   e0066000    and r6, r6, r0
3753     3164:   e0077001    and r7, r7, r1
3754     3168:   e1a00006    mov r0, r6
3755     316c:   e1a01007    mov r1, r7
3756     3170:   e1903001    orrs    r3, r0, r1
3757     3174:   1a000094    bne 33cc <mmap_add_region_ctx+0x2bc>
3758     3178:   e5941008    ldr r1, [r4, #8]
3759     317c:   e18c3001    orr r3, ip, r1
3760     3180:   e7eb3053    ubfx    r3, r3, #0, #12
3761     3184:   e3530000    cmp r3, #0
3762     3188:   1a00008f    bne 33cc <mmap_add_region_ctx+0x2bc>
3763     318c:   e3520101    cmp r2, #1073741824 ; 0x40000000
3764     3190:   13520602    cmpne   r2, #2097152    ; 0x200000
3765     3194:   13a03001    movne   r3, #1
3766     3198:   03a03000    moveq   r3, #0
3767     319c:   e3520a01    cmp r2, #4096   ; 0x1000
3768     31a0:   03a03000    moveq   r3, #0
3769     31a4:   12033001    andne   r3, r3, #1
3770     31a8:   e3530000    cmp r3, #0
3771     31ac:   1a000086    bne 33cc <mmap_add_region_ctx+0x2bc>
3772     31b0:   e1cd20d0    ldrd    r2, [sp]
3773     31b4:   e2522001    subs    r2, r2, #1
3774     31b8:   e2c33000    sbc r3, r3, #0
3775     31bc:   e092600c    adds    r6, r2, ip
3776     31c0:   e2a37000    adc r7, r3, #0
3777     31c4:   e08c3001    add r3, ip, r1
3778     31c8:   e1cd60f8    strd    r6, [sp, #8]
3779     31cc:   e2436001    sub r6, r3, #1
3780     31d0:   e58d301c    str r3, [sp, #28]
3781     31d4:   e1cd20d0    ldrd    r2, [sp]
3782     31d8:   e1cd80d8    ldrd    r8, [sp, #8]
3783     31dc:   e1530009    cmp r3, r9
3784     31e0:   01520008    cmpeq   r2, r8
3785     31e4:   83a03001    movhi   r3, #1
3786     31e8:   93a03000    movls   r3, #0
3787     31ec:   e1510006    cmp r1, r6
3788     31f0:   83833001    orrhi   r3, r3, #1
3789     31f4:   e3530000    cmp r3, #0
3790     31f8:   1a00007a    bne 33e8 <mmap_add_region_ctx+0x2d8>
3791     31fc:   e5953008    ldr r3, [r5, #8]
3792     3200:   e1560003    cmp r6, r3
3793     3204:   8a000077    bhi 33e8 <mmap_add_region_ctx+0x2d8>
3794     3208:   e1c520d0    ldrd    r2, [r5]
3795     320c:   e1590003    cmp r9, r3
3796     3210:   01580002    cmpeq   r8, r2
3797     3214:   8a000073    bhi 33e8 <mmap_add_region_ctx+0x2d8>
3798     3218:   e3a02018    mov r2, #24
3799     321c:   e595300c    ldr r3, [r5, #12]
3800     3220:   e595b010    ldr fp, [r5, #16]
3801     3224:   e0020b92    mul r2, r2, fp
3802     3228:   e58d2018    str r2, [sp, #24]
3803     322c:   e0832002    add r2, r3, r2
3804     3230:   e512200c    ldr r2, [r2, #-12]
3805     3234:   e3520000    cmp r2, #0
3806     3238:   1a00006c    bne 33f0 <mmap_add_region_ctx+0x2e0>
3807     323c:   e1cd80d0    ldrd    r8, [sp]
3808     3240:   e2832018    add r2, r3, #24
3809     3244:   e0518008    subs    r8, r1, r8
3810     3248:   e2e99000    rsc r9, r9, #0
3811     324c:   e1cd82f0    strd    r8, [sp, #32]
3812     3250:   e512000c    ldr r0, [r2, #-12]
3813     3254:   e3500000    cmp r0, #0
3814     3258:   1a000020    bne 32e0 <mmap_add_region_ctx+0x1d0>
3815     325c:   e1a07003    mov r7, r3
3816     3260:   e5972008    ldr r2, [r7, #8]
3817     3264:   e2870018    add r0, r7, #24
3818     3268:   e597100c    ldr r1, [r7, #12]
3819     326c:   e0812002    add r2, r1, r2
3820     3270:   e2422001    sub r2, r2, #1
3821     3274:   e3510000    cmp r1, #0
3822     3278:   11520006    cmpne   r2, r6
3823     327c:   3a00005d    bcc 33f8 <mmap_add_region_ctx+0x2e8>
3824     3280:   e5972008    ldr r2, [r7, #8]
3825     3284:   e2870018    add r0, r7, #24
3826     3288:   e597100c    ldr r1, [r7, #12]
3827     328c:   e081e002    add lr, r1, r2
3828     3290:   e59d201c    ldr r2, [sp, #28]
3829     3294:   e15c0001    cmp ip, r1
3830     3298:   e04e2002    sub r2, lr, r2
3831     329c:   e16f2f12    clz r2, r2
3832     32a0:   e1a022a2    lsr r2, r2, #5
3833     32a4:   93a02000    movls   r2, #0
3834     32a8:   e3520000    cmp r2, #0
3835     32ac:   1a000053    bne 3400 <mmap_add_region_ctx+0x2f0>
3836     32b0:   e59d2018    ldr r2, [sp, #24]
3837     32b4:   e1a01007    mov r1, r7
3838     32b8:   e083b002    add fp, r3, r2
3839     32bc:   e04b2007    sub r2, fp, r7
3840     32c0:   ebfffd21    bl  274c <memmove>
3841     32c4:   e59b300c    ldr r3, [fp, #12]
3842     32c8:   e3530000    cmp r3, #0
3843     32cc:   0a00004d    beq 3408 <mmap_add_region_ctx+0x2f8>
3844     32d0:   e59f2178    ldr r2, [pc, #376]  ; 3450 <mmap_add_region_ctx+0x340>
3845     32d4:   e300131e    movw    r1, #798    ; 0x31e
3846     32d8:   e59f016c    ldr r0, [pc, #364]  ; 344c <mmap_add_region_ctx+0x33c>
3847     32dc:   ebfffcff    bl  26e0 <__assert>
3848     32e0:   e14281d8    ldrd    r8, [r2, #-24]  ; 0xffffffe8
3849     32e4:   e512e010    ldr lr, [r2, #-16]
3850     32e8:   e1cd81f0    strd    r8, [sp, #16]
3851     32ec:   e08e7000    add r7, lr, r0
3852     32f0:   e2477001    sub r7, r7, #1
3853     32f4:   e1560007    cmp r6, r7
3854     32f8:   83a08000    movhi   r8, #0
3855     32fc:   93a08001    movls   r8, #1
3856     3300:   e151000e    cmp r1, lr
3857     3304:   33a08000    movcc   r8, #0
3858     3308:   e3580000    cmp r8, #0
3859     330c:   1a00001f    bne 3390 <mmap_add_region_ctx+0x280>
3860     3310:   e1560007    cmp r6, r7
3861     3314:   33a08000    movcc   r8, #0
3862     3318:   23a08001    movcs   r8, #1
3863     331c:   e151000e    cmp r1, lr
3864     3320:   83a08000    movhi   r8, #0
3865     3324:   e3580000    cmp r8, #0
3866     3328:   1a000018    bne 3390 <mmap_add_region_ctx+0x280>
3867     332c:   e1cda0d0    ldrd    sl, [sp]
3868     3330:   e1cd81d0    ldrd    r8, [sp, #16]
3869     3334:   e2588001    subs    r8, r8, #1
3870     3338:   e2c99000    sbc r9, r9, #0
3871     333c:   e0988000    adds    r8, r8, r0
3872     3340:   e2a99000    adc r9, r9, #0
3873     3344:   e15b0009    cmp fp, r9
3874     3348:   015a0008    cmpeq   sl, r8
3875     334c:   e1cda0d8    ldrd    sl, [sp, #8]
3876     3350:   e1cd81d0    ldrd    r8, [sp, #16]
3877     3354:   83a00001    movhi   r0, #1
3878     3358:   93a00000    movls   r0, #0
3879     335c:   e159000b    cmp r9, fp
3880     3360:   0158000a    cmpeq   r8, sl
3881     3364:   83800001    orrhi   r0, r0, #1
3882     3368:   e1510007    cmp r1, r7
3883     336c:   93a07000    movls   r7, #0
3884     3370:   83a07001    movhi   r7, #1
3885     3374:   e156000e    cmp r6, lr
3886     3378:   21a0e007    movcs   lr, r7
3887     337c:   3387e001    orrcc   lr, r7, #1
3888     3380:   e35e0000    cmp lr, #0
3889     3384:   1a00000d    bne 33c0 <mmap_add_region_ctx+0x2b0>
3890     3388:   e3e01000    mvn r1, #0
3891     338c:   ea00000f    b   33d0 <mmap_add_region_ctx+0x2c0>
3892     3390:   e1cd81d0    ldrd    r8, [sp, #16]
3893     3394:   e1cda2d0    ldrd    sl, [sp, #32]
3894     3398:   e05e8008    subs    r8, lr, r8
3895     339c:   e2e99000    rsc r9, r9, #0
3896     33a0:   e159000b    cmp r9, fp
3897     33a4:   0158000a    cmpeq   r8, sl
3898     33a8:   1afffff6    bne 3388 <mmap_add_region_ctx+0x278>
3899     33ac:   e15c0000    cmp ip, r0
3900     33b0:   0151000e    cmpeq   r1, lr
3901     33b4:   0afffff3    beq 3388 <mmap_add_region_ctx+0x278>
3902     33b8:   e2822018    add r2, r2, #24
3903     33bc:   eaffffa3    b   3250 <mmap_add_region_ctx+0x140>
3904     33c0:   e3500000    cmp r0, #0
3905     33c4:   1afffffb    bne 33b8 <mmap_add_region_ctx+0x2a8>
3906     33c8:   eaffffee    b   3388 <mmap_add_region_ctx+0x278>
3907     33cc:   e3e01015    mvn r1, #21
3908     33d0:   e59f007c    ldr r0, [pc, #124]  ; 3454 <mmap_add_region_ctx+0x344>
3909     33d4:   ebfffb0a    bl  2004 <tf_log>
3910     33d8:   e59f2078    ldr r2, [pc, #120]  ; 3458 <mmap_add_region_ctx+0x348>
3911     33dc:   e30012f1    movw    r1, #753    ; 0x2f1
3912     33e0:   e59f0064    ldr r0, [pc, #100]  ; 344c <mmap_add_region_ctx+0x33c>
3913     33e4:   ebfffcbd    bl  26e0 <__assert>
3914     33e8:   e3e01021    mvn r1, #33 ; 0x21
3915     33ec:   eafffff7    b   33d0 <mmap_add_region_ctx+0x2c0>
3916     33f0:   e3e0100b    mvn r1, #11
3917     33f4:   eafffff5    b   33d0 <mmap_add_region_ctx+0x2c0>
3918     33f8:   e1a07000    mov r7, r0
3919     33fc:   eaffff97    b   3260 <mmap_add_region_ctx+0x150>
3920     3400:   e1a07000    mov r7, r0
3921     3404:   eaffff9d    b   3280 <mmap_add_region_ctx+0x170>
3922     3408:   e8b4000f    ldm r4!, {r0, r1, r2, r3}
3923     340c:   e8a7000f    stmia   r7!, {r0, r1, r2, r3}
3924     3410:   e8940003    ldm r4, {r0, r1}
3925     3414:   e1c522d8    ldrd    r2, [r5, #40]   ; 0x28
3926     3418:   e8870003    stm r7, {r0, r1}
3927     341c:   e1cd00d8    ldrd    r0, [sp, #8]
3928     3420:   e1530001    cmp r3, r1
3929     3424:   01520000    cmpeq   r2, r0
3930     3428:   31a03001    movcc   r3, r1
3931     342c:   31a02000    movcc   r2, r0
3932     3430:   31c522f8    strdcc  r2, [r5, #40]   ; 0x28
3933     3434:   e5953030    ldr r3, [r5, #48]   ; 0x30
3934     3438:   e1530006    cmp r3, r6
3935     343c:   35856030    strcc   r6, [r5, #48]   ; 0x30
3936     3440:   e28dd02c    add sp, sp, #44 ; 0x2c
3937     3444:   e8bd8ff0    pop {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3938     3448:   000053c0    .word   0x000053c0
3939     344c:   000050ff    .word   0x000050ff
3940     3450:   000053fd    .word   0x000053fd
3941     3454:   000053d2    .word   0x000053d2
3942     3458:   000054a2    .word   0x000054a2
3943 
3944 0000345c <mmap_add_region>:
3945     345c:   e92d407f    push    {r0, r1, r2, r3, r4, r5, r6, lr}
3946     3460:   e1cd00f0    strd    r0, [sp]
3947     3464:   e59f0020    ldr r0, [pc, #32]   ; 348c <mmap_add_region+0x30>
3948     3468:   e1a0100d    mov r1, sp
3949     346c:   e1cd20f8    strd    r2, [sp, #8]
3950     3470:   e59d3020    ldr r3, [sp, #32]
3951     3474:   e58d3010    str r3, [sp, #16]
3952     3478:   e3a03101    mov r3, #1073741824 ; 0x40000000
3953     347c:   e58d3014    str r3, [sp, #20]
3954     3480:   ebffff22    bl  3110 <mmap_add_region_ctx>
3955     3484:   e28dd01c    add sp, sp, #28
3956     3488:   e49df004    pop {pc}        ; (ldr pc, [sp], #4)
3957     348c:   0e02e048    .word   0x0e02e048
3958 
3959 00003490 <mmap_add_ctx>:
3960     3490:   e92d4070    push    {r4, r5, r6, lr}
3961     3494:   e1a05000    mov r5, r0
3962     3498:   e1a04001    mov r4, r1
3963     349c:   e594300c    ldr r3, [r4, #12]
3964     34a0:   e3530000    cmp r3, #0
3965     34a4:   08bd8070    popeq   {r4, r5, r6, pc}
3966     34a8:   e1a01004    mov r1, r4
3967     34ac:   e1a00005    mov r0, r5
3968     34b0:   ebffff16    bl  3110 <mmap_add_region_ctx>
3969     34b4:   e2844018    add r4, r4, #24
3970     34b8:   eafffff7    b   349c <mmap_add_ctx+0xc>
3971 
3972 000034bc <mmap_add>:
3973     34bc:   e1a01000    mov r1, r0
3974     34c0:   e59f0000    ldr r0, [pc]    ; 34c8 <mmap_add+0xc>
3975     34c4:   eafffff1    b   3490 <mmap_add_ctx>
3976     34c8:   0e02e048    .word   0x0e02e048
3977 
3978 000034cc <xlat_tables_print>:
3979     34cc:   e590303c    ldr r3, [r0, #60]   ; 0x3c
3980     34d0:   e92d4037    push    {r0, r1, r2, r4, r5, lr}
3981     34d4:   e1a04000    mov r4, r0
3982     34d8:   e3530000    cmp r3, #0
3983     34dc:   0a000006    beq 34fc <xlat_tables_print+0x30>
3984     34e0:   e3530001    cmp r3, #1
3985     34e4:   059f50a8    ldreq   r5, [pc, #168]  ; 3594 <xlat_tables_print+0xc8>
3986     34e8:   0a000004    beq 3500 <xlat_tables_print+0x34>
3987     34ec:   e59f20a4    ldr r2, [pc, #164]  ; 3598 <xlat_tables_print+0xcc>
3988     34f0:   e30014a9    movw    r1, #1193   ; 0x4a9
3989     34f4:   e59f00a0    ldr r0, [pc, #160]  ; 359c <xlat_tables_print+0xd0>
3990     34f8:   ebfffc78    bl  26e0 <__assert>
3991     34fc:   e59f509c    ldr r5, [pc, #156]  ; 35a0 <xlat_tables_print+0xd4>
3992     3500:   e59f009c    ldr r0, [pc, #156]  ; 35a4 <xlat_tables_print+0xd8>
3993     3504:   ebfffabe    bl  2004 <tf_log>
3994     3508:   e1a01005    mov r1, r5
3995     350c:   e59f0094    ldr r0, [pc, #148]  ; 35a8 <xlat_tables_print+0xdc>
3996     3510:   ebfffabb    bl  2004 <tf_log>
3997     3514:   e59f0090    ldr r0, [pc, #144]  ; 35ac <xlat_tables_print+0xe0>
3998     3518:   e1c420d0    ldrd    r2, [r4]
3999     351c:   ebfffab8    bl  2004 <tf_log>
4000     3520:   e59f0088    ldr r0, [pc, #136]  ; 35b0 <xlat_tables_print+0xe4>
4001     3524:   e5941008    ldr r1, [r4, #8]
4002     3528:   ebfffab5    bl  2004 <tf_log>
4003     352c:   e59f0080    ldr r0, [pc, #128]  ; 35b4 <xlat_tables_print+0xe8>
4004     3530:   e1c422d8    ldrd    r2, [r4, #40]   ; 0x28
4005     3534:   ebfffab2    bl  2004 <tf_log>
4006     3538:   e59f0078    ldr r0, [pc, #120]  ; 35b8 <xlat_tables_print+0xec>
4007     353c:   e5941030    ldr r1, [r4, #48]   ; 0x30
4008     3540:   ebfffaaf    bl  2004 <tf_log>
4009     3544:   e59f0070    ldr r0, [pc, #112]  ; 35bc <xlat_tables_print+0xf0>
4010     3548:   e5941034    ldr r1, [r4, #52]   ; 0x34
4011     354c:   ebfffaac    bl  2004 <tf_log>
4012     3550:   e59f0068    ldr r0, [pc, #104]  ; 35c0 <xlat_tables_print+0xf4>
4013     3554:   e5941024    ldr r1, [r4, #36]   ; 0x24
4014     3558:   ebfffaa9    bl  2004 <tf_log>
4015     355c:   e5942018    ldr r2, [r4, #24]
4016     3560:   e594101c    ldr r1, [r4, #28]
4017     3564:   e59f0058    ldr r0, [pc, #88]   ; 35c4 <xlat_tables_print+0xf8>
4018     3568:   e0423001    sub r3, r2, r1
4019     356c:   ebfffaa4    bl  2004 <tf_log>
4020     3570:   e5942020    ldr r2, [r4, #32]
4021     3574:   e3a01000    mov r1, #0
4022     3578:   e1a00004    mov r0, r4
4023     357c:   e5943034    ldr r3, [r4, #52]   ; 0x34
4024     3580:   e58d3000    str r3, [sp]
4025     3584:   e5943024    ldr r3, [r4, #36]   ; 0x24
4026     3588:   ebfffd57    bl  2aec <xlat_tables_print_internal>
4027     358c:   e28dd00c    add sp, sp, #12
4028     3590:   e8bd8030    pop {r4, r5, pc}
4029     3594:   000055af    .word   0x000055af
4030     3598:   0000520a    .word   0x0000520a
4031     359c:   000050ff    .word   0x000050ff
4032     35a0:   000055d3    .word   0x000055d3
4033     35a4:   000055d7    .word   0x000055d7
4034     35a8:   000055f3    .word   0x000055f3
4035     35ac:   0000560d    .word   0x0000560d
4036     35b0:   00005629    .word   0x00005629
4037     35b4:   00005641    .word   0x00005641
4038     35b8:   0000565d    .word   0x0000565d
4039     35bc:   00005675    .word   0x00005675
4040     35c0:   00005692    .word   0x00005692
4041     35c4:   000056b8    .word   0x000056b8
4042 
4043 000035c8 <init_xlat_tables_ctx>:
4044     35c8:   e92d43f7    push    {r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
4045     35cc:   e2504000    subs    r4, r0, #0
4046     35d0:   059f2188    ldreq   r2, [pc, #392]  ; 3760 <init_xlat_tables_ctx+0x198>
4047     35d4:   030014cc    movweq  r1, #1228   ; 0x4cc
4048     35d8:   0a000004    beq 35f0 <init_xlat_tables_ctx+0x28>
4049     35dc:   e5946038    ldr r6, [r4, #56]   ; 0x38
4050     35e0:   e3560000    cmp r6, #0
4051     35e4:   0a000003    beq 35f8 <init_xlat_tables_ctx+0x30>
4052     35e8:   e59f2174    ldr r2, [pc, #372]  ; 3764 <init_xlat_tables_ctx+0x19c>
4053     35ec:   e30014cd    movw    r1, #1229   ; 0x4cd
4054     35f0:   e59f0170    ldr r0, [pc, #368]  ; 3768 <init_xlat_tables_ctx+0x1a0>
4055     35f4:   ebfffc39    bl  26e0 <__assert>
4056     35f8:   e594303c    ldr r3, [r4, #60]   ; 0x3c
4057     35fc:   e3530001    cmp r3, #1
4058     3600:   859f2164    ldrhi   r2, [pc, #356]  ; 376c <init_xlat_tables_ctx+0x1a4>
4059     3604:   830014ce    movwhi  r1, #1230   ; 0x4ce
4060     3608:   8afffff8    bhi 35f0 <init_xlat_tables_ctx+0x28>
4061     360c:   ebfffc9c    bl  2884 <is_mmu_enabled_ctx>
4062     3610:   e3500000    cmp r0, #0
4063     3614:   159f2154    ldrne   r2, [pc, #340]  ; 3770 <init_xlat_tables_ctx+0x1a8>
4064     3618:   130014cf    movwne  r1, #1231   ; 0x4cf
4065     361c:   1afffff3    bne 35f0 <init_xlat_tables_ctx+0x28>
4066     3620:   e594500c    ldr r5, [r4, #12]
4067     3624:   e1a00005    mov r0, r5
4068     3628:   ebfffe9b    bl  309c <print_mmap>
4069     362c:   f2c00010    vmov.i32    d16, #0 ; 0x00000000
4070     3630:   e5941024    ldr r1, [r4, #36]   ; 0x24
4071     3634:   e1a03006    mov r3, r6
4072     3638:   e1510003    cmp r1, r3
4073     363c:   1a000021    bne 36c8 <init_xlat_tables_ctx+0x100>
4074     3640:   f2c00010    vmov.i32    d16, #0 ; 0x00000000
4075     3644:   e5941018    ldr r1, [r4, #24]
4076     3648:   e3000ff8    movw    r0, #4088   ; 0xff8
4077     364c:   e1510006    cmp r1, r6
4078     3650:   1a000021    bne 36dc <init_xlat_tables_ctx+0x114>
4079     3654:   e595300c    ldr r3, [r5, #12]
4080     3658:   e3530000    cmp r3, #0
4081     365c:   1a000028    bne 3704 <init_xlat_tables_ctx+0x13c>
4082     3660:   e1c460d0    ldrd    r6, [r4]
4083     3664:   ebfffc83    bl  2878 <xlat_arch_get_max_supported_pa>
4084     3668:   e1570001    cmp r7, r1
4085     366c:   01560000    cmpeq   r6, r0
4086     3670:   859f20fc    ldrhi   r2, [pc, #252]  ; 3774 <init_xlat_tables_ctx+0x1ac>
4087     3674:   83a01e4f    movhi   r1, #1264   ; 0x4f0
4088     3678:   8affffdc    bhi 35f0 <init_xlat_tables_ctx+0x28>
4089     367c:   e5943008    ldr r3, [r4, #8]
4090     3680:   e5942030    ldr r2, [r4, #48]   ; 0x30
4091     3684:   e1520003    cmp r2, r3
4092     3688:   859f20e8    ldrhi   r2, [pc, #232]  ; 3778 <init_xlat_tables_ctx+0x1b0>
4093     368c:   830014f1    movwhi  r1, #1265   ; 0x4f1
4094     3690:   8affffd6    bhi 35f0 <init_xlat_tables_ctx+0x28>
4095     3694:   e1c420d0    ldrd    r2, [r4]
4096     3698:   e1c402d8    ldrd    r0, [r4, #40]   ; 0x28
4097     369c:   e1510003    cmp r1, r3
4098     36a0:   01500002    cmpeq   r0, r2
4099     36a4:   859f20d0    ldrhi   r2, [pc, #208]  ; 377c <init_xlat_tables_ctx+0x1b4>
4100     36a8:   830014f2    movwhi  r1, #1266   ; 0x4f2
4101     36ac:   8affffcf    bhi 35f0 <init_xlat_tables_ctx+0x28>
4102     36b0:   e3a03001    mov r3, #1
4103     36b4:   e1a00004    mov r0, r4
4104     36b8:   e5843038    str r3, [r4, #56]   ; 0x38
4105     36bc:   e28dd00c    add sp, sp, #12
4106     36c0:   e8bd43f0    pop {r4, r5, r6, r7, r8, r9, lr}
4107     36c4:   eaffff80    b   34cc <xlat_tables_print>
4108     36c8:   e5942020    ldr r2, [r4, #32]
4109     36cc:   e0822183    add r2, r2, r3, lsl #3
4110     36d0:   e2833001    add r3, r3, #1
4111     36d4:   edc20b00    vstr    d16, [r2]
4112     36d8:   eaffffd6    b   3638 <init_xlat_tables_ctx+0x70>
4113     36dc:   e5943014    ldr r3, [r4, #20]
4114     36e0:   e0833606    add r3, r3, r6, lsl #12
4115     36e4:   e2432008    sub r2, r3, #8
4116     36e8:   e0833000    add r3, r3, r0
4117     36ec:   ec598b30    vmov    r8, r9, d16
4118     36f0:   e1e280f8    strd    r8, [r2, #8]!
4119     36f4:   e1520003    cmp r2, r3
4120     36f8:   1afffffb    bne 36ec <init_xlat_tables_ctx+0x124>
4121     36fc:   e2866001    add r6, r6, #1
4122     3700:   eaffffd1    b   364c <init_xlat_tables_ctx+0x84>
4123     3704:   e5943034    ldr r3, [r4, #52]   ; 0x34
4124     3708:   e3a02000    mov r2, #0
4125     370c:   e1a01005    mov r1, r5
4126     3710:   e1a00004    mov r0, r4
4127     3714:   e58d3004    str r3, [sp, #4]
4128     3718:   e5943024    ldr r3, [r4, #36]   ; 0x24
4129     371c:   e58d3000    str r3, [sp]
4130     3720:   e5943020    ldr r3, [r4, #32]
4131     3724:   ebfffda6    bl  2dc4 <xlat_tables_map_region>
4132     3728:   e5951008    ldr r1, [r5, #8]
4133     372c:   e595200c    ldr r2, [r5, #12]
4134     3730:   e0813002    add r3, r1, r2
4135     3734:   e2433001    sub r3, r3, #1
4136     3738:   e1530000    cmp r3, r0
4137     373c:   0a000005    beq 3758 <init_xlat_tables_ctx+0x190>
4138     3740:   e5953010    ldr r3, [r5, #16]
4139     3744:   e59f0034    ldr r0, [pc, #52]   ; 3780 <init_xlat_tables_ctx+0x1b8>
4140     3748:   e1cd20f0    strd    r2, [sp]
4141     374c:   e1c520d0    ldrd    r2, [r5]
4142     3750:   ebfffa2b    bl  2004 <tf_log>
4143     3754:   eb0000fc    bl  3b4c <do_panic>
4144     3758:   e2855018    add r5, r5, #24
4145     375c:   eaffffbc    b   3654 <init_xlat_tables_ctx+0x8c>
4146     3760:   000051d8    .word   0x000051d8
4147     3764:   000053c0    .word   0x000053c0
4148     3768:   000050ff    .word   0x000050ff
4149     376c:   00005286    .word   0x00005286
4150     3770:   000052cb    .word   0x000052cb
4151     3774:   00005330    .word   0x00005330
4152     3778:   00005368    .word   0x00005368
4153     377c:   0000538b    .word   0x0000538b
4154     3780:   000052e4    .word   0x000052e4
4155 
4156 00003784 <init_xlat_tables>:
4157     3784:   e59f0000    ldr r0, [pc]    ; 378c <init_xlat_tables+0x8>
4158     3788:   eaffff8e    b   35c8 <init_xlat_tables_ctx>
4159     378c:   0e02e048    .word   0x0e02e048
4160 
4161 00003790 <enable_mmu_secure>:
4162     3790:   e92d4007    push    {r0, r1, r2, lr}
4163     3794:   e59f1018    ldr r1, [pc, #24]   ; 37b4 <enable_mmu_secure+0x24>
4164     3798:   e5913008    ldr r3, [r1, #8]
4165     379c:   e58d3000    str r3, [sp]
4166     37a0:   e1c122d8    ldrd    r2, [r1, #40]   ; 0x28
4167     37a4:   e5911020    ldr r1, [r1, #32]
4168     37a8:   ebfffc38    bl  2890 <enable_mmu_arch>
4169     37ac:   e28dd00c    add sp, sp, #12
4170     37b0:   e49df004    pop {pc}        ; (ldr pc, [sp], #4)
4171     37b4:   0e02e048    .word   0x0e02e048
4172 
4173 000037b8 <semihosting_call>:
4174     37b8:   ef123456    svc 0x00123456
4175     37bc:   e12fff1e    bx  lr
4176 
4177 000037c0 <plat_my_core_pos>:
4178     37c0:   ee100fb0    mrc 15, 0, r0, cr0, cr0, {5}
4179     37c4:   eaffffff    b   37c8 <plat_qemu_calc_core_pos>
4180 
4181 000037c8 <plat_qemu_calc_core_pos>:
4182     37c8:   e20010ff    and r1, r0, #255    ; 0xff
4183     37cc:   e2000cff    and r0, r0, #65280  ; 0xff00
4184     37d0:   e0810320    add r0, r1, r0, lsr #6
4185     37d4:   e12fff1e    bx  lr
4186 
4187 000037d8 <plat_is_my_cpu_primary>:
4188     37d8:   ee100fb0    mrc 15, 0, r0, cr0, cr0, {5}
4189     37dc:   e59f1010    ldr r1, [pc, #16]   ; 37f4 <plat_is_my_cpu_primary+0x1c>
4190     37e0:   e0000001    and r0, r0, r1
4191     37e4:   e3500000    cmp r0, #0
4192     37e8:   03a00001    moveq   r0, #1
4193     37ec:   13a00000    movne   r0, #0
4194     37f0:   e12fff1e    bx  lr
4195     37f4:   0000ffff    .word   0x0000ffff
4196 
4197 000037f8 <plat_secondary_cold_boot_setup>:
4198     37f8:   ebfffff0    bl  37c0 <plat_my_core_pos>
4199     37fc:   e1a00180    lsl r0, r0, #3
4200     3800:   e3002008    movw    r2, #8
4201     3804:   e3402e00    movt    r2, #3584   ; 0xe00
4202 
4203 00003808 <poll_mailbox>:
4204     3808:   e7921000    ldr r1, [r2, r0]
4205     380c:   e3510000    cmp r1, #0
4206     3810:   0a000003    beq 3824 <poll_mailbox+0x1c>
4207     3814:   e3000000    movw    r0, #0
4208     3818:   e3400e00    movt    r0, #3584   ; 0xe00
4209     381c:   e5901000    ldr r1, [r0]
4210     3820:   e12fff11    bx  r1
4211     3824:   e320f002    wfe
4212     3828:   eafffff6    b   3808 <poll_mailbox>
4213 
4214 0000382c <plat_get_my_entrypoint>:
4215     382c:   e3a00000    mov r0, #0
4216     3830:   e12fff1e    bx  lr
4217 
4218 00003834 <platform_mem_init>:
4219     3834:   e12fff1e    bx  lr
4220 
4221 00003838 <plat_crash_console_init>:
4222     3838:   e3000000    movw    r0, #0
4223     383c:   e3400904    movt    r0, #2308   ; 0x904
4224     3840:   e3a01001    mov r1, #1
4225     3844:   e30c2200    movw    r2, #49664  ; 0xc200
4226     3848:   e3402001    movt    r2, #1
4227     384c:   ea000160    b   3dd4 <console_core_init>
4228 
4229 00003850 <plat_crash_console_putc>:
4230     3850:   e3001000    movw    r1, #0
4231     3854:   e3401904    movt    r1, #2308   ; 0x904
4232     3858:   ea000177    b   3e3c <console_core_putc>
4233 
4234 0000385c <cortex_a15_enable_smp>:
4235     385c:   ee110f30    mrc 15, 0, r0, cr1, cr0, {1}
4236     3860:   e3800040    orr r0, r0, #64 ; 0x40
4237     3864:   ee010f30    mcr 15, 0, r0, cr1, cr0, {1}
4238     3868:   f57ff06f    isb sy
4239     386c:   e12fff1e    bx  lr
4240 
4241 00003870 <check_errata_cve_2017_5715>:
4242     3870:   e3a00001    mov r0, #1
4243     3874:   e12fff1e    bx  lr
4244 
4245 00003878 <cortex_a15_errata_report>:
4246     3878:   e92d5000    push    {ip, lr}
4247     387c:   eb0000a4    bl  3b14 <cpu_get_rev_var>
4248     3880:   e1a04000    mov r4, r0
4249     3884:   e1a00004    mov r0, r4
4250     3888:   ebfffff8    bl  3870 <check_errata_cve_2017_5715>
4251     388c:   e59f100c    ldr r1, [pc, #12]   ; 38a0 <cortex_a15_errata_report+0x28>
4252     3890:   e59f200c    ldr r2, [pc, #12]   ; 38a4 <cortex_a15_errata_report+0x2c>
4253     3894:   ebfff88a    bl  1ac4 <errata_print_msg>
4254     3898:   e8bd5000    pop {ip, lr}
4255     389c:   e12fff1e    bx  lr
4256     38a0:   00005749    .word   0x00005749
4257     38a4:   0000573b    .word   0x0000573b
4258 
4259 000038a8 <cortex_a15_reset_func>:
4260     38a8:   eaffffeb    b   385c <cortex_a15_enable_smp>
4261 
4262 000038ac <bl1_aarch32_smc_handler>:
4263     38ac:   e58de080    str lr, [sp, #128]  ; 0x80
4264     38b0:   e3a0e004    mov lr, #4
4265     38b4:   e15e0000    cmp lr, r0
4266     38b8:   1a000013    bne 390c <smc_handler>
4267     38bc:   ee118f11    mrc 15, 0, r8, cr1, cr1, {0}
4268     38c0:   e3180001    tst r8, #1
4269     38c4:   1b0000af    blne    3b88 <report_exception>
4270     38c8:   e1a08001    mov r8, r1
4271     38cc:   e1a00001    mov r0, r1
4272     38d0:   ebfff7be    bl  17d0 <bl1_print_next_bl_ep_info>
4273     38d4:   e1a00008    mov r0, r8
4274     38d8:   eb000123    bl  3d6c <bl1_plat_prepare_exit>
4275     38dc:   ee080f17    mcr 15, 0, r0, cr8, cr7, {0}
4276     38e0:   f57ff04f    dsb sy
4277     38e4:   f57ff06f    isb sy
4278     38e8:   e598e008    ldr lr, [r8, #8]
4279     38ec:   e598100c    ldr r1, [r8, #12]
4280     38f0:   e169f001    msr SPSR_fc, r1
4281     38f4:   f1020013    cps #19
4282     38f8:   e598e010    ldr lr, [r8, #16]
4283     38fc:   f1020016    cps #22
4284     3900:   e2888014    add r8, r8, #20
4285     3904:   e898000f    ldm r8, {r0, r1, r2, r3}
4286     3908:   e1b0f00e    movs    pc, lr
4287 
4288 0000390c <smc_handler>:
4289     390c:   e88d1fff    stm sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
4290     3910:   e1a0000d    mov r0, sp
4291     3914:   e2800034    add r0, r0, #52 ; 0x34
4292     3918:   ee114f11    mrc 15, 0, r4, cr1, cr1, {0}
4293     391c:   e3c42001    bic r2, r4, #1
4294     3920:   ee012f11    mcr 15, 0, r2, cr1, cr1, {0}
4295     3924:   f57ff06f    isb sy
4296     3928:   f102001f    cps #31
4297     392c:   e8a06000    stmia   r0!, {sp, lr}
4298     3930:   f1020012    cps #18
4299     3934:   e14f2000    mrs r2, SPSR
4300     3938:   e8a06004    stmia   r0!, {r2, sp, lr}
4301     393c:   f1020011    cps #17
4302     3940:   e14f2000    mrs r2, SPSR
4303     3944:   e8a06004    stmia   r0!, {r2, sp, lr}
4304     3948:   f1020013    cps #19
4305     394c:   e14f2000    mrs r2, SPSR
4306     3950:   e8a06004    stmia   r0!, {r2, sp, lr}
4307     3954:   f1020017    cps #23
4308     3958:   e14f2000    mrs r2, SPSR
4309     395c:   e8a06004    stmia   r0!, {r2, sp, lr}
4310     3960:   f102001b    cps #27
4311     3964:   e14f2000    mrs r2, SPSR
4312     3968:   e8a06004    stmia   r0!, {r2, sp, lr}
4313     396c:   f1020016    cps #22
4314     3970:   e14f2000    mrs r2, SPSR
4315     3974:   e8a00004    stmia   r0!, {r2}
4316     3978:   ee014f11    mcr 15, 0, r4, cr1, cr1, {0}
4317     397c:   f57ff06f    isb sy
4318     3980:   e58d4084    str r4, [sp, #132]  ; 0x84
4319     3984:   ee194f1c    mrc 15, 0, r4, cr9, cr12, {0}
4320     3988:   e58d4088    str r4, [sp, #136]  ; 0x88
4321     398c:   e1a0600d    mov r6, sp
4322     3990:   e596d07c    ldr sp, [r6, #124]  ; 0x7c
4323     3994:   e5960084    ldr r0, [r6, #132]  ; 0x84
4324     3998:   e2007001    and r7, r0, #1
4325     399c:   e3c00001    bic r0, r0, #1
4326     39a0:   ee010f11    mcr 15, 0, r0, cr1, cr1, {0}
4327     39a4:   f57ff06f    isb sy
4328     39a8:   e3170001    tst r7, #1
4329     39ac:   1a000005    bne 39c8 <skip_mmu_on>
4330     39b0:   e3a00001    mov r0, #1
4331     39b4:   ebffff75    bl  3790 <enable_mmu_secure>
4332     39b8:   ee119f10    mrc 15, 0, r9, cr1, cr0, {0}
4333     39bc:   e3899004    orr r9, r9, #4
4334     39c0:   ee019f10    mcr 15, 0, r9, cr1, cr0, {0}
4335     39c4:   f57ff06f    isb sy
4336 
4337 000039c8 <skip_mmu_on>:
4338     39c8:   e5960000    ldr r0, [r6]
4339     39cc:   e3a01000    mov r1, #0
4340     39d0:   e1a02006    mov r2, r6
4341     39d4:   e1a03007    mov r3, r7
4342     39d8:   ebfff7a1    bl  1864 <bl1_smc_wrapper>
4343     39dc:   ebfff7c3    bl  18f0 <smc_get_next_ctx>
4344     39e0:   e1a04000    mov r4, r0
4345     39e4:   e5945084    ldr r5, [r4, #132]  ; 0x84
4346     39e8:   e3150001    tst r5, #1
4347     39ec:   1a000003    bne 3a00 <skip_mmu_off>
4348     39f0:   eb0000d6    bl  3d50 <disable_mmu_icache_secure>
4349     39f4:   ee080f17    mcr 15, 0, r0, cr8, cr7, {0}
4350     39f8:   f57ff04f    dsb sy
4351     39fc:   f57ff06f    isb sy
4352 
4353 00003a00 <skip_mmu_off>:
4354     3a00:   e1a00004    mov r0, r4
4355     3a04:   e580d07c    str sp, [r0, #124]  ; 0x7c
4356     3a08:   e1a0d000    mov sp, r0
4357     3a0c:   e5901084    ldr r1, [r0, #132]  ; 0x84
4358     3a10:   ee011f11    mcr 15, 0, r1, cr1, cr1, {0}
4359     3a14:   f57ff06f    isb sy
4360     3a18:   e5901088    ldr r1, [r0, #136]  ; 0x88
4361     3a1c:   ee091f1c    mcr 15, 0, r1, cr9, cr12, {0}
4362     3a20:   e2801034    add r1, r0, #52 ; 0x34
4363     3a24:   ee114f11    mrc 15, 0, r4, cr1, cr1, {0}
4364     3a28:   e3c42001    bic r2, r4, #1
4365     3a2c:   ee012f11    mcr 15, 0, r2, cr1, cr1, {0}
4366     3a30:   f57ff06f    isb sy
4367     3a34:   f102001f    cps #31
4368     3a38:   e8b16000    ldm r1!, {sp, lr}
4369     3a3c:   f1020012    cps #18
4370     3a40:   e8b16004    ldm r1!, {r2, sp, lr}
4371     3a44:   e16ff002    msr SPSR_fsxc, r2
4372     3a48:   f1020011    cps #17
4373     3a4c:   e8b16004    ldm r1!, {r2, sp, lr}
4374     3a50:   e16ff002    msr SPSR_fsxc, r2
4375     3a54:   f1020013    cps #19
4376     3a58:   e8b16004    ldm r1!, {r2, sp, lr}
4377     3a5c:   e16ff002    msr SPSR_fsxc, r2
4378     3a60:   f1020017    cps #23
4379     3a64:   e8b16004    ldm r1!, {r2, sp, lr}
4380     3a68:   e16ff002    msr SPSR_fsxc, r2
4381     3a6c:   f102001b    cps #27
4382     3a70:   e8b16004    ldm r1!, {r2, sp, lr}
4383     3a74:   e16ff002    msr SPSR_fsxc, r2
4384     3a78:   f1020016    cps #22
4385     3a7c:   e8b10004    ldm r1!, {r2}
4386     3a80:   e16ff002    msr SPSR_fsxc, r2
4387     3a84:   ee014f11    mcr 15, 0, r4, cr1, cr1, {0}
4388     3a88:   f57ff06f    isb sy
4389     3a8c:   e590e080    ldr lr, [r0, #128]  ; 0x80
4390     3a90:   e8901fff    ldm r0, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
4391     3a94:   e1b0f00e    movs    pc, lr
4392 
4393 00003a98 <reset_handler>:
4394     3a98:   e1a0a00e    mov sl, lr
4395     3a9c:   eb0000b1    bl  3d68 <plat_reset_handler>
4396     3aa0:   eb00000a    bl  3ad0 <get_cpu_ops_ptr>
4397     3aa4:   e3500000    cmp r0, #0
4398     3aa8:   1a000002    bne 3ab8 <reset_handler+0x20>
4399     3aac:   e59f0018    ldr r0, [pc, #24]   ; 3acc <reset_handler+0x34>
4400     3ab0:   e3a01022    mov r1, #34 ; 0x22
4401     3ab4:   ea000038    b   3b9c <asm_assert>
4402     3ab8:   e5901004    ldr r1, [r0, #4]
4403     3abc:   e3510000    cmp r1, #0
4404     3ac0:   e1a0e00a    mov lr, sl
4405     3ac4:   112fff11    bxne    r1
4406     3ac8:   e12fff1e    bx  lr
4407     3acc:   0000577f    .word   0x0000577f
4408 
4409 00003ad0 <get_cpu_ops_ptr>:
4410     3ad0:   e59f4030    ldr r4, [pc, #48]   ; 3b08 <error_exit+0x4>
4411     3ad4:   e59f5030    ldr r5, [pc, #48]   ; 3b0c <error_exit+0x8>
4412     3ad8:   e3a00000    mov r0, #0
4413     3adc:   ee102f10    mrc 15, 0, r2, cr0, cr0, {0}
4414     3ae0:   e59f3028    ldr r3, [pc, #40]   ; 3b10 <error_exit+0xc>
4415     3ae4:   e0022003    and r2, r2, r3
4416     3ae8:   e1540005    cmp r4, r5
4417     3aec:   2a000004    bcs 3b04 <error_exit>
4418     3af0:   e494100c    ldr r1, [r4], #12
4419     3af4:   e0011003    and r1, r1, r3
4420     3af8:   e1510002    cmp r1, r2
4421     3afc:   1afffff9    bne 3ae8 <get_cpu_ops_ptr+0x18>
4422     3b00:   e244000c    sub r0, r4, #12
4423 
4424 00003b04 <error_exit>:
4425     3b04:   e12fff1e    bx  lr
4426     3b08:   000057e8    .word   0x000057e8
4427     3b0c:   000057f4    .word   0x000057f4
4428     3b10:   ff00fff0    .word   0xff00fff0
4429 
4430 00003b14 <cpu_get_rev_var>:
4431     3b14:   ee101f10    mrc 15, 0, r1, cr0, cr0, {0}
4432     3b18:   e7e70851    ubfx    r0, r1, #16, #8
4433     3b1c:   e7c30011    bfi r0, r1, #0, #4
4434     3b20:   e12fff1e    bx  lr
4435 
4436 00003b24 <print_errata_status>:
4437     3b24:   e92d5030    push    {r4, r5, ip, lr}
4438     3b28:   ebffffe8    bl  3ad0 <get_cpu_ops_ptr>
4439     3b2c:   e5900008    ldr r0, [r0, #8]
4440     3b30:   e3500000    cmp r0, #0
4441     3b34:   112fff30    blxne   r0
4442     3b38:   e8bd9030    pop {r4, r5, ip, pc}
4443 
4444 00003b3c <plat_set_my_stack>:
4445     3b3c:   e59f0004    ldr r0, [pc, #4]    ; 3b48 <plat_set_my_stack+0xc>
4446     3b40:   e1a0d000    mov sp, r0
4447     3b44:   e12fff1e    bx  lr
4448     3b48:   0e02f0c0    .word   0x0e02f0c0
4449 
4450 00003b4c <do_panic>:
4451     3b4c:   e24e6004    sub r6, lr, #4
4452     3b50:   ebffff38    bl  3838 <plat_crash_console_init>
4453     3b54:   e3500000    cmp r0, #0
4454     3b58:   0a000006    beq 3b78 <do_panic+0x2c>
4455     3b5c:   e59f401c    ldr r4, [pc, #28]   ; 3b80 <do_panic+0x34>
4456     3b60:   eb000023    bl  3bf4 <asm_print_str>
4457     3b64:   e1a04006    mov r4, r6
4458     3b68:   eb000028    bl  3c10 <asm_print_hex>
4459     3b6c:   e59f4010    ldr r4, [pc, #16]   ; 3b84 <do_panic+0x38>
4460     3b70:   eb00001f    bl  3bf4 <asm_print_str>
4461     3b74:   eb000079    bl  3d60 <plat_crash_console_flush>
4462     3b78:   e1a0e006    mov lr, r6
4463     3b7c:   ea00007b    b   3d70 <plat_panic_handler>
4464     3b80:   0000579e    .word   0x0000579e
4465     3b84:   000057af    .word   0x000057af
4466 
4467 00003b88 <report_exception>:
4468     3b88:   e10f0000    mrs r0, CPSR
4469     3b8c:   e200001f    and r0, r0, #31
4470     3b90:   eb000071    bl  3d5c <plat_report_exception>
4471     3b94:   eb000075    bl  3d70 <plat_panic_handler>
4472     3b98:   e320f000    nop {0}
4473 
4474 00003b9c <asm_assert>:
4475     3b9c:   e1a05000    mov r5, r0
4476     3ba0:   e1a06001    mov r6, r1
4477     3ba4:   ebffff23    bl  3838 <plat_crash_console_init>
4478     3ba8:   e3500000    cmp r0, #0
4479     3bac:   0a00000b    beq 3be0 <asm_assert+0x44>
4480     3bb0:   e59f4030    ldr r4, [pc, #48]   ; 3be8 <asm_assert+0x4c>
4481     3bb4:   eb00000e    bl  3bf4 <asm_print_str>
4482     3bb8:   e1a04005    mov r4, r5
4483     3bbc:   eb00000c    bl  3bf4 <asm_print_str>
4484     3bc0:   e59f4024    ldr r4, [pc, #36]   ; 3bec <asm_assert+0x50>
4485     3bc4:   eb00000a    bl  3bf4 <asm_print_str>
4486     3bc8:   e59f4020    ldr r4, [pc, #32]   ; 3bf0 <asm_assert+0x54>
4487     3bcc:   e1160004    tst r6, r4
4488     3bd0:   1a000002    bne 3be0 <asm_assert+0x44>
4489     3bd4:   e1a04006    mov r4, r6
4490     3bd8:   eb00000c    bl  3c10 <asm_print_hex>
4491     3bdc:   eb00005f    bl  3d60 <plat_crash_console_flush>
4492     3be0:   eb000062    bl  3d70 <plat_panic_handler>
4493     3be4:   e320f000    nop {0}
4494     3be8:   000057b2    .word   0x000057b2
4495     3bec:   000057c0    .word   0x000057c0
4496     3bf0:   ffff0000    .word   0xffff0000
4497 
4498 00003bf4 <asm_print_str>:
4499     3bf4:   e1a0300e    mov r3, lr
4500     3bf8:   e4d40001    ldrb    r0, [r4], #1
4501     3bfc:   e3500000    cmp r0, #0
4502     3c00:   0a000001    beq 3c0c <asm_print_str+0x18>
4503     3c04:   ebffff11    bl  3850 <plat_crash_console_putc>
4504     3c08:   eafffffa    b   3bf8 <asm_print_str+0x4>
4505     3c0c:   e12fff13    bx  r3
4506 
4507 00003c10 <asm_print_hex>:
4508     3c10:   e1a0300e    mov r3, lr
4509     3c14:   e3a05020    mov r5, #32
4510     3c18:   e2455004    sub r5, r5, #4
4511     3c1c:   e1a00534    lsr r0, r4, r5
4512     3c20:   e200000f    and r0, r0, #15
4513     3c24:   e350000a    cmp r0, #10
4514     3c28:   3a000000    bcc 3c30 <asm_print_hex+0x20>
4515     3c2c:   e2800027    add r0, r0, #39 ; 0x27
4516     3c30:   e2800030    add r0, r0, #48 ; 0x30
4517     3c34:   ebffff05    bl  3850 <plat_crash_console_putc>
4518     3c38:   e3550000    cmp r5, #0
4519     3c3c:   1afffff5    bne 3c18 <asm_print_hex+0x8>
4520     3c40:   e12fff13    bx  r3
4521 
4522 00003c44 <flush_dcache_range>:
4523     3c44:   e3510000    cmp r1, #0
4524     3c48:   0a00000b    beq 3c7c <exit_loop_cimvac>
4525     3c4c:   ee103f30    mrc 15, 0, r3, cr0, cr0, {1}
4526     3c50:   e7e33853    ubfx    r3, r3, #16, #4
4527     3c54:   e3a02004    mov r2, #4
4528     3c58:   e1a02312    lsl r2, r2, r3
4529     3c5c:   e0801001    add r1, r0, r1
4530     3c60:   e2423001    sub r3, r2, #1
4531     3c64:   e1c00003    bic r0, r0, r3
4532 
4533 00003c68 <loop_cimvac>:
4534     3c68:   ee070f3e    mcr 15, 0, r0, cr7, cr14, {1}
4535     3c6c:   e0800002    add r0, r0, r2
4536     3c70:   e1500001    cmp r0, r1
4537     3c74:   3afffffb    bcc 3c68 <loop_cimvac>
4538     3c78:   f57ff04f    dsb sy
4539 
4540 00003c7c <exit_loop_cimvac>:
4541     3c7c:   e12fff1e    bx  lr
4542 
4543 00003c80 <zero_normalmem>:
4544     3c80:   e3a02000    mov r2, #0
4545     3c84:   e0801001    add r1, r0, r1
4546     3c88:   e3100007    tst r0, #7
4547     3c8c:   0a000007    beq 3cb0 <zero_normalmem+0x30>
4548     3c90:   e380c007    orr ip, r0, #7
4549     3c94:   e29cc001    adds    ip, ip, #1
4550     3c98:   0a00000b    beq 3ccc <zero_normalmem+0x4c>
4551     3c9c:   e15c0001    cmp ip, r1
4552     3ca0:   2a000009    bcs 3ccc <zero_normalmem+0x4c>
4553     3ca4:   e4c02001    strb    r2, [r0], #1
4554     3ca8:   e150000c    cmp r0, ip
4555     3cac:   1afffffc    bne 3ca4 <zero_normalmem+0x24>
4556     3cb0:   e3c1c007    bic ip, r1, #7
4557     3cb4:   e150000c    cmp r0, ip
4558     3cb8:   2a000003    bcs 3ccc <zero_normalmem+0x4c>
4559     3cbc:   e3a03000    mov r3, #0
4560     3cc0:   e8a0000c    stmia   r0!, {r2, r3}
4561     3cc4:   e150000c    cmp r0, ip
4562     3cc8:   3afffffc    bcc 3cc0 <zero_normalmem+0x40>
4563     3ccc:   e1500001    cmp r0, r1
4564     3cd0:   0a000002    beq 3ce0 <zero_normalmem+0x60>
4565     3cd4:   e4c02001    strb    r2, [r0], #1
4566     3cd8:   e1500001    cmp r0, r1
4567     3cdc:   1afffffc    bne 3cd4 <zero_normalmem+0x54>
4568     3ce0:   e12fff1e    bx  lr
4569 
4570 00003ce4 <memcpy4>:
4571     3ce4:   e1803001    orr r3, r0, r1
4572     3ce8:   e3130003    tst r3, #3
4573     3cec:   0a000002    beq 3cfc <m_loop4>
4574     3cf0:   e59f0038    ldr r0, [pc, #56]   ; 3d30 <m_end+0x4>
4575     3cf4:   e3a01090    mov r1, #144    ; 0x90
4576     3cf8:   eaffffa7    b   3b9c <asm_assert>
4577 
4578 00003cfc <m_loop4>:
4579     3cfc:   e3520004    cmp r2, #4
4580     3d00:   3a000003    bcc 3d14 <m_loop1>
4581     3d04:   e4913004    ldr r3, [r1], #4
4582     3d08:   e4803004    str r3, [r0], #4
4583     3d0c:   e2422004    sub r2, r2, #4
4584     3d10:   eafffff9    b   3cfc <m_loop4>
4585 
4586 00003d14 <m_loop1>:
4587     3d14:   e3520000    cmp r2, #0
4588     3d18:   0a000003    beq 3d2c <m_end>
4589     3d1c:   e4d13001    ldrb    r3, [r1], #1
4590     3d20:   e4c03001    strb    r3, [r0], #1
4591     3d24:   e2522001    subs    r2, r2, #1
4592     3d28:   1afffff9    bne 3d14 <m_loop1>
4593 
4594 00003d2c <m_end>:
4595     3d2c:   e12fff1e    bx  lr
4596     3d30:   000057c9    .word   0x000057c9
4597 
4598 00003d34 <disable_mmu_secure>:
4599     3d34:   e3a01005    mov r1, #5
4600 
4601 00003d38 <do_disable_mmu>:
4602     3d38:   ee110f10    mrc 15, 0, r0, cr1, cr0, {0}
4603     3d3c:   e1c00001    bic r0, r0, r1
4604     3d40:   ee010f10    mcr 15, 0, r0, cr1, cr0, {0}
4605     3d44:   f57ff06f    isb sy
4606     3d48:   f57ff04f    dsb sy
4607     3d4c:   e12fff1e    bx  lr
4608 
4609 00003d50 <disable_mmu_icache_secure>:
4610     3d50:   e51f1000    ldr r1, [pc, #-0]   ; 3d58 <disable_mmu_icache_secure+0x8>
4611     3d54:   eafffff7    b   3d38 <do_disable_mmu>
4612     3d58:   00001005    .word   0x00001005
4613 
4614 00003d5c <plat_report_exception>:
4615     3d5c:   e12fff1e    bx  lr
4616 
4617 00003d60 <plat_crash_console_flush>:
4618     3d60:   e3a00000    mov r0, #0
4619     3d64:   e12fff1e    bx  lr
4620 
4621 00003d68 <plat_reset_handler>:
4622     3d68:   e12fff1e    bx  lr
4623 
4624 00003d6c <bl1_plat_prepare_exit>:
4625     3d6c:   e12fff1e    bx  lr
4626 
4627 00003d70 <plat_panic_handler>:
4628     3d70:   eafffffe    b   3d70 <plat_panic_handler>
4629 
4630 00003d74 <__aeabi_uldivmod>:
4631     3d74:   e92d4040    push    {r6, lr}
4632     3d78:   e24dd010    sub sp, sp, #16
4633     3d7c:   e28d6008    add r6, sp, #8
4634     3d80:   e58d6000    str r6, [sp]
4635     3d84:   ebfff990    bl  23cc <__udivmoddi4>
4636     3d88:   e59d2008    ldr r2, [sp, #8]
4637     3d8c:   e59d300c    ldr r3, [sp, #12]
4638     3d90:   e28dd010    add sp, sp, #16
4639     3d94:   e8bd8040    pop {r6, pc}
4640 
4641 00003d98 <console_init>:
4642     3d98:   e3500000    cmp r0, #0
4643     3d9c:   0a000002    beq 3dac <init_fail>
4644     3da0:   e59f3008    ldr r3, [pc, #8]    ; 3db0 <init_fail+0x4>
4645     3da4:   e5830000    str r0, [r3]
4646     3da8:   ea000009    b   3dd4 <console_core_init>
4647 
4648 00003dac <init_fail>:
4649     3dac:   e12fff1e    bx  lr
4650     3db0:   0e02e088    .word   0x0e02e088
4651 
4652 00003db4 <console_putc>:
4653     3db4:   e59f2004    ldr r2, [pc, #4]    ; 3dc0 <console_putc+0xc>
4654     3db8:   e5921000    ldr r1, [r2]
4655     3dbc:   ea00001e    b   3e3c <console_core_putc>
4656     3dc0:   0e02e088    .word   0x0e02e088
4657 
4658 00003dc4 <console_flush>:
4659     3dc4:   e59f1004    ldr r1, [pc, #4]    ; 3dd0 <console_flush+0xc>
4660     3dc8:   e5910000    ldr r0, [r1]
4661     3dcc:   ea00002a    b   3e7c <console_core_flush>
4662     3dd0:   0e02e088    .word   0x0e02e088
4663 
4664 00003dd4 <console_core_init>:
4665     3dd4:   e3500000    cmp r0, #0
4666     3dd8:   0a000014    beq 3e30 <core_init_fail>
4667     3ddc:   e3510000    cmp r1, #0
4668     3de0:   0a000012    beq 3e30 <core_init_fail>
4669     3de4:   e3520000    cmp r2, #0
4670     3de8:   0a000010    beq 3e30 <core_init_fail>
4671     3dec:   e5903030    ldr r3, [r0, #48]   ; 0x30
4672     3df0:   e3c33001    bic r3, r3, #1
4673     3df4:   e5803030    str r3, [r0, #48]   ; 0x30
4674     3df8:   e1a01101    lsl r1, r1, #2
4675     3dfc:   e732f211    udiv    r2, r1, r2
4676     3e00:   e1a01322    lsr r1, r2, #6
4677     3e04:   e5801024    str r1, [r0, #36]   ; 0x24
4678     3e08:   e202103f    and r1, r2, #63 ; 0x3f
4679     3e0c:   e5801028    str r1, [r0, #40]   ; 0x28
4680     3e10:   e3a01070    mov r1, #112    ; 0x70
4681     3e14:   e580102c    str r1, [r0, #44]   ; 0x2c
4682     3e18:   e3a01000    mov r1, #0
4683     3e1c:   e5801004    str r1, [r0, #4]
4684     3e20:   e59f1010    ldr r1, [pc, #16]   ; 3e38 <core_init_fail+0x8>
4685     3e24:   e5801030    str r1, [r0, #48]   ; 0x30
4686     3e28:   e3a00001    mov r0, #1
4687     3e2c:   e12fff1e    bx  lr
4688 
4689 00003e30 <core_init_fail>:
4690     3e30:   e3a00000    mov r0, #0
4691     3e34:   e12fff1e    bx  lr
4692     3e38:   00000301    .word   0x00000301
4693 
4694 00003e3c <console_core_putc>:
4695     3e3c:   e3510000    cmp r1, #0
4696     3e40:   0a00000b    beq 3e74 <putc_error>
4697     3e44:   e350000a    cmp r0, #10
4698     3e48:   1a000004    bne 3e60 <console_core_putc+0x24>
4699     3e4c:   e5912018    ldr r2, [r1, #24]
4700     3e50:   e3120020    tst r2, #32
4701     3e54:   1afffffc    bne 3e4c <console_core_putc+0x10>
4702     3e58:   e3a0200d    mov r2, #13
4703     3e5c:   e5812000    str r2, [r1]
4704     3e60:   e5912018    ldr r2, [r1, #24]
4705     3e64:   e3120020    tst r2, #32
4706     3e68:   1afffffc    bne 3e60 <console_core_putc+0x24>
4707     3e6c:   e5810000    str r0, [r1]
4708     3e70:   e12fff1e    bx  lr
4709 
4710 00003e74 <putc_error>:
4711     3e74:   e3e00000    mvn r0, #0
4712     3e78:   e12fff1e    bx  lr
4713 
4714 00003e7c <console_core_flush>:
4715     3e7c:   e3500000    cmp r0, #0
4716     3e80:   0a000004    beq 3e98 <flush_error>
4717     3e84:   e5901018    ldr r1, [r0, #24]
4718     3e88:   e3110008    tst r1, #8
4719     3e8c:   1afffffc    bne 3e84 <console_core_flush+0x8>
4720     3e90:   e3a00000    mov r0, #0
4721     3e94:   e12fff1e    bx  lr
4722 
4723 00003e98 <flush_error>:
4724     3e98:   e3e00000    mvn r0, #0
4725     3e9c:   e12fff1e    bx  lr
4726 
4727 00003ea0 <bl1_vector_table>:
4728     3ea0:   eafff056    b   0 <bl1_entrypoint>
4729     3ea4:   eaffff37    b   3b88 <report_exception>
4730     3ea8:   eafffe7f    b   38ac <bl1_aarch32_smc_handler>
4731     3eac:   eaffff35    b   3b88 <report_exception>
4732     3eb0:   eaffff34    b   3b88 <report_exception>
4733     3eb4:   eaffff33    b   3b88 <report_exception>
4734     3eb8:   eaffff32    b   3b88 <report_exception>
4735     3ebc:   eaffff31    b   3b88 <report_exception>
4736     ...