/*
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/*
    Adreno Ramper is designed specially for Xperia Z5, x0r3d and CarbonKernel project
    It does ramp up a frequency when the work is demanding
    Uses some source code from Adreno Idler
    Copyright Mark Hargreaves
    GNU GPLv2
*/

// xor3d: Check for Adreno Idler init
// nergzd723: We dont need such since the modules are different
// #ifdef CONFIG_ADRENO_IDLER
// #error Adreno Idler is enabled. Choose between Idler and Ramper.
// #endif

#include <linux/module.h>
#include <linux/devfreq.h>
#include <linux/msm_adreno_devfreq.h>

#define ADRENO_RAMPER_MAJOR_VERSION 0
#define ADRENO_RAMPER_MINOR_VERSION 1

static unsigned long nonidleworkload = 20000;
module_param_named(adreno_ramper_nonidleworkload, nonidleworkload, ulong, 0664);
static unsigned int rampwait = 15;
module_param_named(adreno_ramper_rampwait, rampwait, uint, 0664);
static unsigned int downdifferential = 20;
module_param_named(adreno_ramper_downdifferential, downdifferential, uint, 0664);
static bool adreno_ramper_active = true;
module_param_named(adreno_ramper_active, adreno_ramper_active, bool, 0664);
static unsigned int idlecount = 0;
int adreno_ramper(struct devfreq_dev_status stats, struct devfreq *devfreq,
		 unsigned long *freq)
{
	if (!adreno_ramper_active) return 0;
	if (!adreno_ramper_active)
	{
		return 0;
	}
	if (stats.busy_time > nonidleworkload) {
		idlecount++;
		if (*freq == devfreq->profile->freq_table[devfreq->profile->max_state - 1]) {
			/* Frequency is already at its lowest.
			   No need to calculate things, so bail out. */
			return 1;
		}
		if (idlecount > rampwait &&
		    stats.busy_time * 100 > stats.total_time * downdifferential) {
			/* We are idle for (idlewait + 1)'th time! Ramp down the frequency now. */
			*freq = devfreq->profile->freq_table[devfreq->profile->max_state - 1];
            		printk("Got ramped!\n");
			idlecount--;
			return 1;
		} else {
			idlecount = 0;
			/* Do not return 1 here and allow rest of the algorithm to
			   figure out the appropriate frequency for current workload.
			   It can even set it back to the lowest frequency. */
		}
	}
	else {
		idlecount = 0;
		/* Do not return 1 here and allow rest of the algorithm to
		   figure out the appropriate frequency for current workload.
		   It can even set it back to the lowest frequency. */
	}
	return 0;
}
EXPORT_SYMBOL(adreno_ramper);

static int __init adreno_ramper_init(void)
{
	pr_info("adreno_ramper: version %d.%d by nergzd723\n",
		 ADRENO_RAMPER_MAJOR_VERSION,
		 ADRENO_RAMPER_MINOR_VERSION);
    	printk("Init!\n");
	return 0;
}
subsys_initcall(adreno_ramper_init);

static void __exit adreno_ramper_exit(void)
{
	return;
}
module_exit(adreno_ramper_exit);

MODULE_AUTHOR("Mark Hargreaves <clashclanacc2602@gmail.com> ");
MODULE_DESCRIPTION("adreno_ramper - A powersaver for Adreno TZ Control fastening algorithm for Adreno GPU series");
MODULE_LICENSE("GPL");
