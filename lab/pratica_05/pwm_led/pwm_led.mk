################################################################################
#
# pwm_led
#
################################################################################

PWM_LED_VERSION = 1.0
PWM_LED_SITE = $(TOPDIR)/package/pwm_led/src
PWM_LED_SITE_METHOD = local

define PWM_LED_BUILD_CMDS
	$(TARGET_CXX) $(TARGET_CXXFLAGS) $(PWM_LED_SITE)/pwm_led.cpp -o $(@D)/pwm_led
endef

define PWM_LED_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/pwm_led $(TARGET_DIR)/usr/bin/pwm_led
endef

$(eval $(generic-package))
