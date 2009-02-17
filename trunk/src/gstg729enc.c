/*
 * GladSToNe
 * Copyright (C) 2009 Gibro Vacco <<gibrovacco@gmail.com>>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Alternatively, the contents of this file may be used under the
 * GNU Lesser General Public License Version 2.1 (the "LGPL"), in
 * which case the following provisions apply instead of the ones
 * mentioned above:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * SECTION:element-g729enc
 *
 * FIXME:Describe g729enc here.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch -v -m fakesrc ! g729enc ! fakesink silent=TRUE
 * ]|
 * </refsect2>
 */

#include <string.h>

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gst/gst.h>

#include "gstg729enc.h"

GST_DEBUG_CATEGORY_STATIC (gst_g729enc_debug);
#define GST_CAT_DEFAULT gst_g729enc_debug

/* Filter signals and args */
enum
{
  /* FILL ME */
  LAST_SIGNAL
};

enum
{
  PROP_0,
  PROP_SILENT
};

/* the capabilities of the inputs and outputs.
 *
 * describe the real formats here.
 */
static GstStaticPadTemplate sink_factory = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("audio/x-raw-int, rate=8000, channels=1")
    );

static GstStaticPadTemplate src_factory = GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("audio/G729, rate=8000, channels=1")
    );

GST_BOILERPLATE (Gstg729enc, gst_g729enc, GstElement,
    GST_TYPE_ELEMENT);

static void gst_g729enc_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);
static void gst_g729enc_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);

static gboolean gst_g729enc_set_caps (GstPad * pad, GstCaps * caps);
static GstFlowReturn gst_g729enc_chain (GstPad * pad, GstBuffer * buf);

/* GObject vmethod implementations */

static void
gst_g729enc_base_init (gpointer gclass)
{
  GstElementClass *element_class = GST_ELEMENT_CLASS (gclass);

  gst_element_class_set_details_simple(element_class,
    "g729enc",
    "FIXME:Generic",
    "FIXME:Generic Template Element",
    "Marco Ballesio <<user@hostname.org>>");

  gst_element_class_add_pad_template (element_class,
      gst_static_pad_template_get (&src_factory));
  gst_element_class_add_pad_template (element_class,
      gst_static_pad_template_get (&sink_factory));
}

/* initialize the g729enc's class */
static void
gst_g729enc_class_init (Gstg729encClass * klass)
{
  GObjectClass *gobject_class;
  GstElementClass *gstelement_class;

  gobject_class = (GObjectClass *) klass;
  gstelement_class = (GstElementClass *) klass;

  gobject_class->set_property = gst_g729enc_set_property;
  gobject_class->get_property = gst_g729enc_get_property;

  g_object_class_install_property (gobject_class, PROP_SILENT,
      g_param_spec_boolean ("silent", "Silent", "Produce verbose output ?",
          FALSE, G_PARAM_READWRITE));
}

/* initialize the new element
 * instantiate pads and add them to element
 * set pad calback functions
 * initialize instance structure
 */
static void
gst_g729enc_init (Gstg729enc * filter,
    Gstg729encClass * gclass)
{
  filter->sinkpad = gst_pad_new_from_static_template (&sink_factory, "sink");
  gst_pad_set_setcaps_function (filter->sinkpad,
                                GST_DEBUG_FUNCPTR(gst_g729enc_set_caps));
  gst_pad_set_getcaps_function (filter->sinkpad,
                                GST_DEBUG_FUNCPTR(gst_pad_proxy_getcaps));
  gst_pad_set_chain_function (filter->sinkpad,
                              GST_DEBUG_FUNCPTR(gst_g729enc_chain));

  filter->srcpad = gst_pad_new_from_static_template (&src_factory, "src");
  gst_pad_set_getcaps_function (filter->srcpad,
                                GST_DEBUG_FUNCPTR(gst_pad_proxy_getcaps));

  gst_element_add_pad (GST_ELEMENT (filter), filter->sinkpad);
  gst_element_add_pad (GST_ELEMENT (filter), filter->srcpad);

  Init_Pre_Process();
  Init_Coder_ld8a();
//  Set_zero(prm, PRM_SIZE);

  filter->silent = FALSE;
}

static void
gst_g729enc_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  Gstg729enc *filter = GST_G729ENC (object);

  switch (prop_id) {
    case PROP_SILENT:
      filter->silent = g_value_get_boolean (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_g729enc_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec)
{
  Gstg729enc *filter = GST_G729ENC (object);

  switch (prop_id) {
    case PROP_SILENT:
      g_value_set_boolean (value, filter->silent);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

/* GstElement vmethod implementations */

/* this function handles the link with other elements */
static gboolean
gst_g729enc_set_caps (GstPad * pad, GstCaps * caps)
{
  Gstg729enc *filter;
  GstPad *otherpad;

  filter = GST_G729ENC (gst_pad_get_parent (pad));
  otherpad = (pad == filter->srcpad) ? filter->sinkpad : filter->srcpad;
  gst_object_unref (filter);

  return gst_pad_set_caps (otherpad, caps);
}

/* chain function
 * this function does the actual processing
 */
static GstFlowReturn
gst_g729enc_chain (GstPad * pad, GstBuffer * buf)
{
  Gstg729enc *filter;
  int pos,i,j,index;
  GstBuffer* outbuf;
  extern Word16 *new_speech;
  Word16 prm[PRM_SIZE],serial[SERIAL_SIZE];
  static Word16 remainder[L_FRAME];
  static int rem_size=0;

  filter = GST_G729ENC (GST_OBJECT_PARENT (pad));

  for(pos=0;pos+L_FRAME*sizeof(Word16)<=GST_BUFFER_SIZE(buf);pos+=L_FRAME*sizeof(Word16)){
    outbuf=gst_buffer_new();
    GST_BUFFER_SIZE(outbuf)=10;
    GST_BUFFER_MALLOCDATA (outbuf) = g_malloc (GST_BUFFER_SIZE(outbuf));
    GST_BUFFER_DATA (outbuf) = GST_BUFFER_MALLOCDATA (outbuf);
    gst_buffer_copy_metadata(outbuf,buf,
      GST_BUFFER_COPY_FLAGS | GST_BUFFER_COPY_TIMESTAMPS | GST_BUFFER_COPY_CAPS);

    if(rem_size>0){
      memcpy(new_speech,remainder,rem_size);
      memcpy(new_speech+rem_size/sizeof(Word16),GST_BUFFER_DATA(buf), sizeof(Word16)*L_FRAME-rem_size);
      pos=-rem_size;
      rem_size=0;
    }else{
      memcpy(new_speech,GST_BUFFER_DATA(buf)+pos, sizeof(Word16)*L_FRAME);
    }

    Pre_Process(new_speech, L_FRAME);
    Coder_ld8a(prm);
    prm2bits_ld8k(prm, serial);

    memset (GST_BUFFER_DATA(outbuf),0x0,GST_BUFFER_SIZE(outbuf));

    for(i=0;i<10;i++){
      for(j=0;j<8;j++){
        index=2+(i*8)+j;
        GST_BUFFER_DATA(outbuf)[i]|=serial[index]==0x81?(1<<(7-j)):0;
//	printf ("%x ",serial[index]);
      }
  //    printf ("->%x \n",GST_BUFFER_DATA(outbuf)[i]);
    }

    gst_pad_push (filter->srcpad, outbuf);
  }

  rem_size=GST_BUFFER_SIZE(buf)-pos;
  memcpy(remainder,GST_BUFFER_DATA(buf)+pos,rem_size);

  gst_buffer_unref (buf);
  return GST_FLOW_OK;
}


/* entry point to initialize the plug-in
 * initialize the plug-in itself
 * register the element factories and other features
 */
static gboolean
g729enc_init (GstPlugin * g729enc)
{
  /* debug category for fltering log messages
   */
  GST_DEBUG_CATEGORY_INIT (gst_g729enc_debug, "g729enc",
      0, "Template g729enc");

  return gst_element_register (g729enc, "g729enc", GST_RANK_NONE,
      GST_TYPE_G729ENC);
}

/* gstreamer looks for this structure to register g729encs
 *
 * exchange the string 'Template g729enc' with your g729enc description
 */
GST_PLUGIN_DEFINE (
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    "g729enc",
    "Template g729enc",
    g729enc_init,
    VERSION,
    "LGPL",
    "GStreamer",
    "http://gstreamer.net/"
)
