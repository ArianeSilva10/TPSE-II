################################################################################
#
# pwm_buzzer2
#
################################################################################

PWM_BUZZER2_VERSION = 4.0
PWM_BUZZER2_SITE = $(TOPDIR)/package/pwm_buzzer2/src
PWM_BUZZER2_SITE_METHOD = local

define PWM_BUZZER2_BUILD_CMDS
    $(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define PWM_BUZZER2_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/pwm_buzzer2 $(TARGET_DIR)/usr/bin
endef

$(eval $(generic-package))