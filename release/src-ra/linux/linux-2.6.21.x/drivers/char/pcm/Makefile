SI3220_DIR = ./si3220_rev1.5
SI3220_OBJ = $(SI3220_DIR)/slic.o $(SI3220_DIR)/isr.o $(SI3220_DIR)/dual_i2o.o $(SI3220_DIR)/data.o \
			$(SI3220_DIR)/slic_data.o $(SI3220_DIR)/countries.o
		
obj-$(CONFIG_RALINK_PCM)  += ralink_pcm.o
ralink_pcm-objs       		:= pcm_ctrl.o ./codec_lib/codec_api.o $(SI3220_OBJ) 



