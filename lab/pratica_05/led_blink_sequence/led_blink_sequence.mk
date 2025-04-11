################################################################################
#
# led_blink_sequence
#
################################################################################

LED_BLINK_SEQUENCE_VERSION = 1.0
LED_BLINK_SEQUENCE_SITE = $(TOPDIR)/package/led_blink_sequence/src
LED_BLINK_SEQUENCE_SITE_METHOD = local

define LED_BLINK_SEQUENCE_BUILD_CMDS
    $(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define LED_BLINK_SEQUENCE_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/led_blink_sequence $(TARGET_DIR)/usr/bin
endef

$(eval $(generic-package))