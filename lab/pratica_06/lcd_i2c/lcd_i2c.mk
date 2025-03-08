################################################################################
#
# lcd_i2c
#
################################################################################

LCD_I2C_VERSION = 1.0
LCD_I2C_SITE = $(TOPDIR)/package/lcd_i2c/src
LCD_I2C_SITE_METHOD = local

define LCD_I2C_BUILD_CMDS
	$(TARGET_CXX) $(TARGET_CXXFLAGS) $(LCD_I2C_SITE)/lcd_i2c.cpp -o $(@D)/lcd_i2c
endef

define LCD_I2C_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/lcd_i2c $(TARGET_DIR)/usr/bin/lcd_i2c
endef

$(eval $(generic-package))
