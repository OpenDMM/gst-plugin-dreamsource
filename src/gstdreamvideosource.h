/*
 * GStreamer dreamvideosource
 * Copyright 2014-2015 Andreas Frisch <fraxinas@opendreambox.org>
 *
 * This program is licensed under the Creative Commons
 * Attribution-NonCommercial-ShareAlike 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to
 * Creative Commons,559 Nathan Abbott Way,Stanford,California 94305,USA.
 *
 * Alternatively, this program may be distributed and executed on
 * hardware which is licensed by Dream Multimedia GmbH.
 *
 * This program is NOT free software. It is open source, you are allowed
 * to modify it (if you keep the license), but it may not be commercially
 * distributed other than under the conditions noted above.
 */

#ifndef __GST_DREAMVIDEOSOURCE_H__
#define __GST_DREAMVIDEOSOURCE_H__

#include "gstdreamsource.h"
#include <gst/video/video.h>

G_BEGIN_DECLS

/* VIDEO field validity flags */
#define VBD_FLAG_DTS_VALID                 0x00000001
/* VIDEO indicator flags */
#define VBD_FLAG_RAP                       0x00010000
/* indicates a video data unit (NALU, EBDU, etc) starts at the beginning of
   this descriptor  - if this is set, then the uiDataUnitID field is valid also */
#define VBD_FLAG_DATA_UNIT_START           0x00020000
#define VBD_FLAG_EXTENDED                  0x80000000

#define VENC_START        _IO('v', 128)
#define VENC_STOP         _IO('v', 129)
#define VENC_SET_BITRATE  _IOW('v', 130, unsigned int)
#define VENC_SET_RESOLUTION _IOW('v', 131, unsigned int)
#define VENC_SET_FRAMERATE  _IOW('v', 132, unsigned int)

enum venc_framerate {
        rate_custom = 0,
        rate_25,
        rate_30,
        rate_50,
        rate_60,
};

enum venc_videoformat {
        fmt_custom = 0,
        fmt_720x576,
        fmt_1280x720,
        fmt_1920x1080,
};
struct _VideoBufferDescriptor
{
	CompressedBufferDescriptor stCommon;
	uint32_t uiVideoFlags;
	uint64_t uiDTS;		/* 33-bit DTS value (in 90 Kh or 27Mhz?) */
	uint8_t uiDataUnitType;
};

struct _VideoFormatInfo {
	gint width;
	gint height;

	gint par_n;	/* pixel-aspect-ratio numerator */
	gint par_d;	/* pixel-aspect-ratio demnominator */
	gint fps_n;	/* framerate numerator */
	gint fps_d;	/* framerate demnominator */

	gint bitrate;
};

#define VBDSIZE 	sizeof(VideoBufferDescriptor)
#define VBUFSIZE	(1024*16)
#define VMMAPSIZE	(1024*1024*6)

#define GST_TYPE_DREAMVIDEOSOURCE \
  (gst_dreamvideosource_get_type())
#define GST_DREAMVIDEOSOURCE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_DREAMVIDEOSOURCE,GstDreamVideoSource))
#define GST_DREAMVIDEOSOURCE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_DREAMVIDEOSOURCE,GstDreamVideoSourceClass))
#define GST_IS_DREAMVIDEOSOURCE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_DREAMVIDEOSOURCE))
#define GST_IS_DREAMVIDEOSOURCE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_DREAMVIDEOSOURCE))

typedef struct _GstDreamVideoSource        GstDreamVideoSource;
typedef struct _GstDreamVideoSourceClass   GstDreamVideoSourceClass;

typedef struct _VideoFormatInfo            VideoFormatInfo;
typedef struct _VideoBufferDescriptor      VideoBufferDescriptor;

// #define dump 1

struct _GstDreamVideoSource
{
	GstPushSrc element;

	EncoderInfo *encoder;

	VideoFormatInfo video_info;

	unsigned int descriptors_available;
	unsigned int descriptors_count;

	int dumpfd;
	
	GstElement *dreamaudiosrc;
	GstClockTime base_pts;
	
	GMutex mutex;
};

struct _GstDreamVideoSourceClass
{
	GstPushSrcClass parent_class;
	gint64 (*get_base_pts) (GstDreamVideoSource *self);
};

GType gst_dreamvideosource_get_type (void);
gboolean gst_dreamvideosource_plugin_init (GstPlugin * plugin);

G_END_DECLS

#endif /* __GST_DREAMVIDEOSOURCE_H__ */

