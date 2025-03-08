################################################################################
#
# pwm_buzzer
#
################################################################################

PWM_BUZZER_VERSION = 2.0
PWM_BUZZER_SITE = $(TOPDIR)/package/pwm_buzzer/src
PWM_BUZZER_SITE_METHOD = local

define PWM_BUZZER_BUILD_CMDS
    $(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define PWM_BUZZER_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/pwm_buzzer $(TARGET_DIR)/usr/bin
endef

$(eval $(generic-package))