################################################################################
#
# pwm_servo
#
################################################################################

PWM_SERVO_VERSION = 1.0
PWM_SERVO_SITE = $(TOPDIR)/package/pwm_servo/src
PWM_SERVO_SITE_METHOD = local

define PWM_SERVO_BUILD_CMDS
	$(TARGET_CXX) $(TARGET_CXXFLAGS) $(PWM_SERVO_SITE)/pwm_servo.cpp -o $(@D)/pwm_servo
endef

define PWM_SERVO_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/pwm_servo $(TARGET_DIR)/usr/bin/pwm_servo
endef

$(eval $(generic-package))
