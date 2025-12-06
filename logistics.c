#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

int main(void) {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) {
        return 0;  // invalid input
    }

    double *dp_to_co = (double *)malloc(n * sizeof(double));
    double *co_to_wh = (double *)malloc(m * sizeof(double));
    double *wh_traffic = (double *)malloc(m * sizeof(double));

    if (!dp_to_co || !co_to_wh || !wh_traffic) {
        // memory allocation failed
        return 0;
    }

    for (int i = 0; i < n; ++i) {
        scanf("%lf", &dp_to_co[i]);
    }
    for (int j = 0; j < m; ++j) {
        scanf("%lf", &co_to_wh[j]);
    }
    for (int j = 0; j < m; ++j) {
        scanf("%lf", &wh_traffic[j]);
    }

    // Find nearest DP by raw distance to consumer
    double min_dp_dist = dp_to_co[0];
    for (int i = 1; i < n; ++i) {
        if (dp_to_co[i] < min_dp_dist) {
            min_dp_dist = dp_to_co[i];
        }
    }

    const double EPS = 1e-9;

    // Best overall pairing
    bool has_overall = false;
    double best_eff_o = 0.0, best_dist_o = 0.0;
    int best_dp_o = -1, best_wh_o = -1;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            double total_dist = dp_to_co[i] + co_to_wh[j];
            double eff_time = total_dist * (1.0 + wh_traffic[j] / 100.0);

            if (!has_overall ||
                eff_time < best_eff_o ||
                (fabs(eff_time - best_eff_o) < EPS && total_dist < best_dist_o)) {
                has_overall = true;
                best_eff_o = eff_time;
                best_dist_o = total_dist;
                best_dp_o = i;
                best_wh_o = j;
            }
        }
    }

    // Best pairing restricted to nearest DP(s)
    bool has_nearest = false;
    double best_eff_n = 0.0, best_dist_n = 0.0;
    int best_dp_n = -1, best_wh_n = -1;

    for (int i = 0; i < n; ++i) {
        if (fabs(dp_to_co[i] - min_dp_dist) < EPS) {  // one of the nearest DPs
            for (int j = 0; j < m; ++j) {
                double total_dist = dp_to_co[i] + co_to_wh[j];
                double eff_time = total_dist * (1.0 + wh_traffic[j] / 100.0);

                if (!has_nearest ||
                    eff_time < best_eff_n ||
                    (fabs(eff_time - best_eff_n) < EPS && total_dist < best_dist_n)) {
                    has_nearest = true;
                    best_eff_n = eff_time;
                    best_dist_n = total_dist;
                    best_dp_n = i;
                    best_wh_n = j;
                }
            }
        }
    }

    // Output results (1-based indices)
    printf("BEST_OVERALL\n");
    printf("DP_index %d\n", best_dp_o + 1);
    printf("WH_index %d\n", best_wh_o + 1);
    printf("Raw_distance %.3f\n", best_dist_o);
    printf("Traffic_pct %.2f\n", wh_traffic[best_wh_o]);
    printf("Effective_time %.3f\n", best_eff_o);

    printf("BEST_WITH_NEAREST_DP\n");
    printf("DP_index %d\n", best_dp_n + 1);
    printf("WH_index %d\n", best_wh_n + 1);
    printf("Raw_distance %.3f\n", best_dist_n);
    printf("Traffic_pct %.2f\n", wh_traffic[best_wh_n]);
    printf("Effective_time %.3f\n", best_eff_n);

    free(dp_to_co);
    free(co_to_wh);
    free(wh_traffic);

    return 0;
}
