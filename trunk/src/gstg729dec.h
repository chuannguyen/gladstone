
#ifndef __GST_G729_DEC_H__
#define __GST_G729_DEC_H__

#include <gst/gst.h>
#include "g729common.h"

//ref code includes:
#include "typedef.h"
#include "ld8a.h"
#include "basic_op.h"

G_BEGIN_DECLS

#define GST_TYPE_G729_DEC \
  (gst_g729_dec_get_type())
#define GST_G729_DEC(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_G729_DEC,GstG729Dec))
#define GST_G729_DEC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_G729_DEC,GstG729DecClass))
#define GST_IS_G729_DEC(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_G729_DEC))
#define GST_IS_G729_DEC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_G729_DEC))

typedef struct _GstG729Dec GstG729Dec;
typedef struct _GstG729DecClass GstG729DecClass;

struct _GstG729Dec {
  GstElement            element;

  /* pads */
  GstPad                *sinkpad;
  GstPad                *srcpad;

  void                  *state;

  guint64               packetno;

  GstSegment            segment;    /* STREAM LOCK */
  gint64                granulepos; /* -1 = needs to be set from current time */

  //ref code specific
  Word16 synth_buf[SERIAL_SIZE+M];
  Word16 *synth; 

};

struct _GstG729DecClass {
  GstElementClass parent_class;
};

GType gst_g729_dec_get_type (void);

G_END_DECLS

#endif /* __GST_G729_DEC_H__ */
