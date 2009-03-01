
#ifndef __GST_G729_ENC_H__
#define __GST_G729_ENC_H__


#include <gst/gst.h>
#include <gst/base/gstadapter.h>
#include "g729common.h"

G_BEGIN_DECLS

#define GST_TYPE_G729_ENC \
  (gst_g729_enc_get_type())
#define GST_G729_ENC(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_G729_ENC,GstG729Enc))
#define GST_G729_ENC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_G729_ENC,GstG729EncClass))
#define GST_IS_G729_ENC(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_G729_ENC))
#define GST_IS_G729_ENC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_G729_ENC))


typedef enum
{
  GST_G729_ENC_ANNEXA,
} GstG729Mode;

typedef struct _GstG729Enc GstG729Enc;
typedef struct _GstG729EncClass GstG729EncClass;

struct _GstG729Enc {
  GstElement            element;

  /* pads */
  GstPad                *sinkpad,
                        *srcpad;

  gint                  packet_count;
  gint                  n_packets;

  void                  *state;
  GstG729Mode          mode;
  GstAdapter            *adapter;

  gboolean              vad;
  gboolean              dtx;

  gboolean              setup;

  guint64               samples_in;
  guint64               bytes_out;

  GstTagList            *tags;

  gchar                 *last_message;

  guint64               frameno;
  guint64               frameno_out;

  guint8                *comments;
  gint                  comment_len;

  /* Timestamp and granulepos tracking */
  GstClockTime     start_ts;
  GstClockTime     next_ts;
  guint64          granulepos_offset;
};

struct _GstG729EncClass {
  GstElementClass parent_class;

  /* signals */
  void (*frame_encoded) (GstElement *element);
};

GType gst_g729_enc_get_type (void);

G_END_DECLS

#endif /* __GST_G729ENC_H__ */
