# plugindir is set in configure

##############################################################################
# change libgstg729enc.la to something more suitable, e.g. libmysomething.la  #
##############################################################################
plugin_LTLIBRARIES = libgstg729enc.la

##############################################################################
# for the next set of variables, rename the prefix if you renamed the .la,   #
#  e.g. libgstg729enc_la_SOURCES => libmysomething_la_SOURCES                 #
#       libgstg729enc_la_CFLAGS  => libmysomething_la_CFLAGS                  #
#       libgstg729enc_la_LIBADD  => libmysomething_la_LIBADD                  #
#       libgstg729enc_la_LDFLAGS => libmysomething_la_LDFLAGS                 #
##############################################################################

# sources used to compile this plug-in
libgstg729enc_la_SOURCES = gstg729enc.c

# flags used to compile this plugin
# add other _CFLAGS and _LIBS as needed
libgstg729enc_la_CFLAGS = $(GST_CFLAGS) -I $(G729_PATH)
libgstg729enc_la_LIBADD = $(GST_LIBS)\
			  $(G729_PATH)\acelp_ca.o\
			  $(G729_PATH)\basic_op.o\
			  $(G729_PATH)\bits.o\
			  $(G729_PATH)\cod_ld8a.o\
			  $(G729_PATH)\coder.o\
			  $(G729_PATH)\dspfunc.o\
			  $(G729_PATH)\filter.o\
			  $(G729_PATH)\gainpred.o\
			  $(G729_PATH)\lpc.o\
			  $(G729_PATH)\lpcfunc.o\
			  $(G729_PATH)\lspgetq.o\
			  $(G729_PATH)\oper_32b.o\
			  $(G729_PATH)\p_parity.o\
			  $(G729_PATH)\pitch_a.o\
			  $(G729_PATH)\pre_proc.o\
			  $(G729_PATH)\pred_lt3.o\
			  $(G729_PATH)\qua_gain.o\
			  $(G729_PATH)\qua_lsp.o\
			  $(G729_PATH)\tab_ld8a.o\
			  $(G729_PATH)\util.o\
			  $(G729_PATH)\taming.o\
			  $(G729_PATH)\cor_func.o

libgstg729enc_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS) -L $(G729_PATH)
libgstg729enc_la_LIBTOOLFLAGS = --tag=disable-static

# headers we need but don't want installed
noinst_HEADERS = gstg729enc.h
