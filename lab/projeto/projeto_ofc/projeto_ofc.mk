################################################################################
#
# PROJETO_OFC
#
################################################################################

PROJETO_OFC_VERSION = 1.0.1
PROJETO_OFC_SITE = $(TOPDIR)/package/projeto_ofc/src
PROJETO_OFC_SITE_METHOD = local

define PROJETO_OFC_BUILD_CMDS
    $(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define PROJETO_OFC_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/projeto_ofc $(TARGET_DIR)/usr/bin
endef

$(eval $(generic-package))