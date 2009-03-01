#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gstg729enc.h"
#include "gstg729dec.h"

GST_DEBUG_CATEGORY (g729enc_debug);
GST_DEBUG_CATEGORY (g729dec_debug);

static gboolean
plugin_init (GstPlugin * plugin)
{
  if (!gst_element_register (plugin, "g729enc", GST_RANK_NONE,
        gst_g729_enc_get_type ()))
    return FALSE;

  if (!gst_element_register (plugin, "g729dec", GST_RANK_PRIMARY,
        gst_g729_dec_get_type ()))
    return FALSE;

  GST_DEBUG_CATEGORY_INIT (g729enc_debug, "g729enc", 0,
      "g729 encoding element");
  GST_DEBUG_CATEGORY_INIT (g729dec_debug, "g729dec", 0,
      "g729 decoding element");

  return TRUE;
}

GST_PLUGIN_DEFINE (
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    "g729",
    "G729 plugin library",
    plugin_init, 
    VERSION, 
    "LGPL", 
    "G729",
    "http://www.google.com"
    )

