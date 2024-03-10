#pragma once

struct mg_tcpip_driver_stm32f_data {
  // MDC clock divider. MDC clock is derived from HCLK, must not exceed 2.5MHz
  //    HCLK range    DIVIDER    mdc_cr VALUE
  //    -------------------------------------
  //                                -1  <-- tell driver to guess the value
  //    60-100 MHz    HCLK/42        0
  //    100-150 MHz   HCLK/62        1
  //    20-35 MHz     HCLK/16        2
  //    35-60 MHz     HCLK/26        3
  //    150-216 MHz   HCLK/102       4  <-- value for Nucleo-F* on max speed
  //    216-310 MHz   HCLK/124       5
  //    110, 111 Reserved
  int mdc_cr;  // Valid values: -1, 0, 1, 2, 3, 4, 5

  uint8_t phy_addr;  // PHY address
};

#ifndef MG_MAC_ADDRESS
#define MG_MAC_ADDRESS MG_MAC_ADDRESS_RANDOM
#endif

#ifndef MG_TCPIP_PHY_ADDR
#define MG_TCPIP_PHY_ADDR 0
#endif

#ifndef MG_TCPIP_MDC_CR
#define MG_TCPIP_MDC_CR 4
#endif

#define MG_TCPIP_DRIVER_INIT(mgr)                                  \
  do {                                                             \
    static struct mg_tcpip_driver_stm32f_data driver_data = {      \
        .mdc_cr = MG_TCPIP_MDC_CR,                                 \
        .phy_addr = MG_TCPIP_PHY_ADDR,                             \
    };                                                             \
    static struct mg_tcpip_if mif = {                              \
        .mac = MG_MAC_ADDRESS,                                     \
        .ip = MG_TCPIP_IP,                                         \
        .mask = MG_TCPIP_MASK,                                     \
        .gw = MG_TCPIP_GW,                                         \
        .driver = &mg_tcpip_driver_stm32f,                         \
        .driver_data = &driver_data,                               \
    };                                                             \
    mg_tcpip_init((mgr), &mif);                                    \
    MG_INFO(("Driver: stm32fxx, MAC: %M", mg_print_mac, mif.mac)); \
  } while (0)
