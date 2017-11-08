/******************************************************************************
    Copyright (C) 2014 by Ruwen Hahn <palana@stunned.de>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include <string.h>
#include <assert.h>
#include <util/bmem.h>
#include <util/c99defs.h>
#include <util/darray.h>

#define PORT 0xD050
#define DATA 0x03f8
#include "obs-internal.h"

obs_lsl_t *obs_lsl_create() {
	struct obs_lsl *obs_lsl;
	obs_lsl = bzalloc(sizeof(struct obs_lsl));
	bool success;
	int  c1 = 4;
	int c2 = 2;
	//success = obs_lsl_initialize_internal_display(obs_lsl, c2);

	success = obs_lsl_initialize_internal_output(obs_lsl, c1);
	obs->obs_lsl_global = obs_lsl;
	obs->obs_lsl_active = true;

	if (success)
		obs_lsl->initialized = true;
		blog(LOG_DEBUG, "lsl '%s' (%s) created", "OBS", "Markers");
	return obs_lsl;
}

static inline bool obs_lsl_initialize_internal_output(obs_lsl_t *obs_lsl,int chan)
{
	lsl_streaminfo info;		/* out stream declaration object */
	lsl_xml_ptr desc, chn, chns;/* some xml element pointers */
	lsl_outlet outlet1;			/* stream outlet */
	double starttime;			/* used for send timing */

	info = lsl_create_streaminfo("OBS Studio", "Markers", chan, 1, cft_double64, "abcdefg");
	char *channel_label[] = { "frame number", "frame time1", "frame time2", "frame time2" };
	char *channel_units[] = { "sample", "seconds", "seconds", "seconds"};
	desc = lsl_get_desc(info);
	lsl_append_child_value(desc, "manufacturer", "obs");
	chns = lsl_append_child(desc, "channels");
	for (int c = 0; c<chan; c++) {
		chn = lsl_append_child(chns, "channel");
		lsl_append_child_value(chn, "label", channel_label[c]);
		lsl_append_child_value(chn, "unit", channel_units[c]);
		lsl_append_child_value(chn, "type", "Markers");
	}

	/* make a new outlet (chunking: default, buffering: 360 seconds) */
	outlet1 = lsl_create_outlet(info, 0, 180);
	obs_lsl->outlet1 = outlet1;
	return true;
}

bool *init_lsl(struct obs_lsl *labStream)
{
	pthread_mutexattr_t attr;

	pthread_mutex_init_value(&labStream->init_mutex);
	pthread_mutex_init_value(&labStream->callbacks_mutex);
	pthread_mutex_init_value(&labStream->outputs_mutex);

	if (pthread_mutexattr_init(&attr) != 0)
		return false;
	if (pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE) != 0)
		return false;
	if (pthread_mutex_init(&labStream->init_mutex, &attr) != 0)
		return false;
	if (pthread_mutex_init(&labStream->callbacks_mutex, &attr) != 0)
		return false;
	if (pthread_mutex_init(&labStream->outputs_mutex, NULL) != 0)
		return false;

	return true;
}

bool obs_lsl_destroy(obs_lsl_t* lslOutput) {
	lsl_destroy_outlet(lslOutput->outlet1);

	lslOutput->initialized = false;
	obs->obs_lsl_active = false;
	return true;
}

void send_lsl_trigger_output(obs_lsl_t* lslOutput, double sample[])
{
	lsl_push_sample_d(lslOutput->outlet1, sample);
}
