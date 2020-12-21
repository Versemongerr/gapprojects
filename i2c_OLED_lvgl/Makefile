# User Test
#------------------------------------

LVCORE = ./lvgl/src/lv_core
LVDRAW = ./lvgl/src/lv_draw
LVFONT = ./lvgl/src/lv_font
LVHAL = ./lvgl/src/lv_hal
LVMISC = ./lvgl/src/lv_misc
LVOBJX = ./lvgl/src/lv_objx
LVTHEMES = ./lvgl/src/lv_themes

APP              = test
APP_SRCS        += ./SRC/main.c ./SRC/OLED.c
APP_INC         += ./INC/. ./lvgl/src/lv_core/. ./lvgl/src/lv_draw/. ./lvgl/src/lv_font/. ./lvgl/src/lv_hal/. ./lvgl/src/lv_misc/. ./lvgl/src/lv_objx/. ./lvgl/src/lv_themes/. ./lvgl/. ./lvgl/src/.
APP_CFLAGS      += -O3 -g

APP_SRCS += $(LVCORE)/lv_group.c
APP_SRCS += $(LVCORE)/lv_indev.c
APP_SRCS += $(LVCORE)/lv_disp.c
APP_SRCS += $(LVCORE)/lv_obj.c
APP_SRCS += $(LVCORE)/lv_refr.c
APP_SRCS += $(LVCORE)/lv_style.c
APP_SRCS += $(LVCORE)/lv_debug.c

APP_SRCS += $(LVDRAW)/lv_draw_basic.c
APP_SRCS += $(LVDRAW)/lv_draw.c
APP_SRCS += $(LVDRAW)/lv_draw_rect.c
APP_SRCS += $(LVDRAW)/lv_draw_label.c
APP_SRCS += $(LVDRAW)/lv_draw_line.c
APP_SRCS += $(LVDRAW)/lv_draw_img.c
APP_SRCS += $(LVDRAW)/lv_draw_arc.c
APP_SRCS += $(LVDRAW)/lv_draw_triangle.c
APP_SRCS += $(LVDRAW)/lv_img_decoder.c
APP_SRCS += $(LVDRAW)/lv_img_cache.c

APP_SRCS += $(LVFONT)/lv_font.c
APP_SRCS += $(LVFONT)/lv_font_fmt_txt.c
APP_SRCS += $(LVFONT)/lv_font_roboto_12.c
APP_SRCS += $(LVFONT)/lv_font_roboto_16.c
APP_SRCS += $(LVFONT)/lv_font_roboto_22.c
APP_SRCS += $(LVFONT)/lv_font_roboto_28.c
APP_SRCS += $(LVFONT)/lv_font_unscii_8.c

APP_SRCS += $(LVHAL)/lv_hal_disp.c
APP_SRCS += $(LVHAL)/lv_hal_indev.c
APP_SRCS += $(LVHAL)/lv_hal_tick.c

APP_SRCS += $(LVMISC)/lv_circ.c
APP_SRCS += $(LVMISC)/lv_area.c
APP_SRCS += $(LVMISC)/lv_task.c
APP_SRCS += $(LVMISC)/lv_fs.c
APP_SRCS += $(LVMISC)/lv_anim.c
APP_SRCS += $(LVMISC)/lv_mem.c
APP_SRCS += $(LVMISC)/lv_ll.c
APP_SRCS += $(LVMISC)/lv_color.c
APP_SRCS += $(LVMISC)/lv_txt.c
APP_SRCS += $(LVMISC)/lv_math.c
APP_SRCS += $(LVMISC)/lv_log.c
APP_SRCS += $(LVMISC)/lv_gc.c
APP_SRCS += $(LVMISC)/lv_utils.c
APP_SRCS += $(LVMISC)/lv_async.c
APP_SRCS += $(LVMISC)/lv_printf.c
APP_SRCS += $(LVMISC)/lv_bidi.c

APP_SRCS += $(LVOBJX)/lv_arc.c
APP_SRCS += $(LVOBJX)/lv_bar.c
APP_SRCS += $(LVOBJX)/lv_cb.c
APP_SRCS += $(LVOBJX)/lv_cpicker.c
APP_SRCS += $(LVOBJX)/lv_ddlist.c
APP_SRCS += $(LVOBJX)/lv_kb.c
APP_SRCS += $(LVOBJX)/lv_line.c
APP_SRCS += $(LVOBJX)/lv_mbox.c
APP_SRCS += $(LVOBJX)/lv_preload.c
APP_SRCS += $(LVOBJX)/lv_roller.c
APP_SRCS += $(LVOBJX)/lv_table.c
APP_SRCS += $(LVOBJX)/lv_tabview.c
APP_SRCS += $(LVOBJX)/lv_tileview.c
APP_SRCS += $(LVOBJX)/lv_btn.c
APP_SRCS += $(LVOBJX)/lv_calendar.c
APP_SRCS += $(LVOBJX)/lv_chart.c
APP_SRCS += $(LVOBJX)/lv_canvas.c
APP_SRCS += $(LVOBJX)/lv_gauge.c
APP_SRCS += $(LVOBJX)/lv_label.c
APP_SRCS += $(LVOBJX)/lv_list.c
APP_SRCS += $(LVOBJX)/lv_slider.c
APP_SRCS += $(LVOBJX)/lv_ta.c
APP_SRCS += $(LVOBJX)/lv_spinbox.c
APP_SRCS += $(LVOBJX)/lv_btnm.c
APP_SRCS += $(LVOBJX)/lv_cont.c
APP_SRCS += $(LVOBJX)/lv_img.c
APP_SRCS += $(LVOBJX)/lv_imgbtn.c
APP_SRCS += $(LVOBJX)/lv_led.c
APP_SRCS += $(LVOBJX)/lv_lmeter.c
APP_SRCS += $(LVOBJX)/lv_page.c
APP_SRCS += $(LVOBJX)/lv_sw.c
APP_SRCS += $(LVOBJX)/lv_win.c

APP_SRCS += $(LVTHEMES)/lv_theme_alien.c
APP_SRCS += $(LVTHEMES)/lv_theme.c
APP_SRCS += $(LVTHEMES)/lv_theme_default.c
APP_SRCS += $(LVTHEMES)/lv_theme_night.c
APP_SRCS += $(LVTHEMES)/lv_theme_templ.c
APP_SRCS += $(LVTHEMES)/lv_theme_zen.c
APP_SRCS += $(LVTHEMES)/lv_theme_material.c
APP_SRCS += $(LVTHEMES)/lv_theme_nemo.c
APP_SRCS += $(LVTHEMES)/lv_theme_mono.c


PMSIS_OS = pulpos
# LVGL_DIR = ./lvgl
PLPBRIDGE_FLAGS += -f $(APP_SRCS) -hyper

include $(RULES_DIR)/pmsis_rules.mk
# include $(LVGL_DIR)/lvgl.mk
