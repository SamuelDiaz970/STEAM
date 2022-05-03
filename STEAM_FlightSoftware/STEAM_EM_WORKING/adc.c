#include <sensors/sensors.h>
#include <stdio.h>

double get_adc_value(const char *adc_name)
{
	if (sensors_init(NULL))
		fprintf(stderr, "Could not init sensors library\n");

	sensors_chip_name match;
	int res = sensors_parse_chip_name(adc_name, &match);
	if (res)
		return res;

	sensors_chip_name const * cn;
	int c = 0;
	while ((cn = sensors_get_detected_chips(&match, &c)) != 0) {
		printf("Chip: %s:%s\n", cn->prefix, cn->path);

		sensors_feature const *feat;
		int f = 0;

		while ((feat = sensors_get_features(cn, &f)) != 0) {
			printf("%d: %s\n", f, feat->name);

			sensors_subfeature const *subf;
			int s = 0;

			while ((subf = sensors_get_all_subfeatures(cn, feat, &s)) != 0) {
				printf("\t%d: %s/%d = ", s, subf->name, subf->number);
				double val;
				if (subf->flags & SENSORS_MODE_R) {
					int rc = sensors_get_value(cn, subf->number, &val);
					if (rc < 0) {
						printf("err: %d\n", rc);
					} else {
						printf("%f\n", val);
					}
				}
			}
		}
	}

	sensors_free_chip_name(&match);
	sensors_cleanup();
	return 0;
}
