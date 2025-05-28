import sys
import matplotlib.pyplot as plt
import matplotlib as mpl
import matplotlib.font_manager
from matplotlib import rc
from pylab import rcParams
import numpy as np
import math
# from labellines import labelLines
# from math import atan2, degrees
# import numpy as np

def setup():
    plt.style.use("ggplot")
    textColor = '#444444'
    fontName = 'Linux Biolinium'
    #mpl.rc('font', family=fontName)
    mpl.rc('font', size=24)
    mpl.rc('font', weight='light')
    #mpl.rc('text', color=textColor)
    mpl.rc('text', hinting='native')
    pgf_with_latex = {
        "pgf.texsystem": "pdflatex",
        "text.usetex": True,
        "font.family": "serif",
        "font.serif": [],
        "font.sans-serif": [],
        "font.monospace": [],
        "axes.titlesize":  24,
        "axes.labelsize":  24,
        "font.size":  12,
        "legend.fontsize":  12,
        "xtick.labelsize":  12,
        "ytick.labelsize":  12,
        # "figure.figsize": figsize(0.9),     # default fig size of 0.9 textwidth
        "pgf.preamble": r"\usepackage[utf8x]{inputenc} \usepackage[T1]{fontenc}"
    }
    mpl.rcParams.update(pgf_with_latex)

# NOTE: this will be ran from the build folder
datapath = "../scenes/unit_tests/web_viewer/clocking_logs/"

# sys.argv[1] - contains the data file to plot

# x-axis modes
DATE_MODE = 1 # plots the x-axis in terms of dates
ENTRY_MODE = 2 # plots the x-axis in terms of number of data entries

X_AXIS_MODE = ENTRY_MODE

class ClockingData:
    def __init__(self, title, save_name, data):
        self.title = title
        self.save_name = save_name
        self.data = data

iters = []
years = []
months = []
days = []
api_clockings = []
rendering_clockings = []
camera_ray_clockings = []
scene_parse_clockings = []
integrator_clockings = []
integrator_intersect_clockings = []
integrator_preprocess_clockings = []
integrator_rr_clockings = []
integrator_nee_clockings = []
integrator_eval_clockings = []
shader_clockings = []
shader_surface_clockings = []
shader_surface_eval_clockings = []
shader_surface_sample_clockings = []
shader_surface_preprocess_clockings = []
shader_media_clockings = []
shader_media_eval_clockings = []
shader_media_sample_clockings = []
shader_media_preprocess_clockings = []
filter_clockings = []
embree_clockings = []
debug_one_clockings = []
debug_two_clockings = []
debug_three_clockings = []

data = open(datapath + sys.argv[1] + ".dat").readlines()

cnt = 1
for line in data:
    line_data = line.split(",")
    if len(line_data) != 27:
        print("parsed clockings do not have the expected 27 data points per line")
        print(line_data)
        print(len(line_data))
        exit()
    
    iters.append(cnt)
    cnt = cnt + 1

    years.append(int(line_data[0]))
    months.append(int(line_data[1]))
    days.append(int(line_data[2]))
    api_clockings.append(float(line_data[3]))
    rendering_clockings.append(float(line_data[4]))
    camera_ray_clockings.append(float(line_data[5]))
    scene_parse_clockings.append(float(line_data[6]))
    integrator_clockings.append(float(line_data[7]))
    integrator_intersect_clockings.append(float(line_data[8]))
    integrator_preprocess_clockings.append(float(line_data[9]))
    integrator_rr_clockings.append(float(line_data[10]))
    integrator_nee_clockings.append(float(line_data[11]))
    integrator_eval_clockings.append(float(line_data[12]))
    shader_clockings.append(float(line_data[13]))
    shader_surface_clockings.append(float(line_data[14]))
    shader_surface_eval_clockings.append(float(line_data[15]))
    shader_surface_sample_clockings.append(float(line_data[16]))
    shader_surface_preprocess_clockings.append(float(line_data[17]))
    shader_media_clockings.append(float(line_data[18]))
    shader_media_eval_clockings.append(float(line_data[19]))
    shader_media_sample_clockings.append(float(line_data[20]))
    shader_media_preprocess_clockings.append(float(line_data[21]))
    filter_clockings.append(float(line_data[22]))
    embree_clockings.append(float(line_data[23]))
    debug_one_clockings.append(float(line_data[24]))
    debug_two_clockings.append(float(line_data[25]))
    debug_three_clockings.append(float(line_data[26]))

api_data = ClockingData("API", "api", api_clockings)
rendering_clockings = ClockingData("Rendering", "rendering", rendering_clockings)
camera_ray_data = ClockingData("Camera Ray", "camera_ray", camera_ray_clockings)
scene_parse_data = ClockingData("Scene Parse", "scene_parse", scene_parse_clockings)
integrator_data = ClockingData("Integrator", "integrator", integrator_clockings)
integrator_intersect_data = ClockingData("Integrator Intersect", "integrator_intersect", integrator_intersect_clockings)
integrator_preprocess_data = ClockingData("Integrator Pre-process", "integrator_pre_process", integrator_preprocess_clockings)
integrator_rr_data = ClockingData("Integrator RR", "integrator_rr", integrator_rr_clockings)
integrator_nee_data = ClockingData("Integrator NEE", "integrator_nee", integrator_nee_clockings)
integrator_eval_data = ClockingData("Integrator Eval", "integrator_eval", integrator_eval_clockings)
shader_data = ClockingData("Shader", "shader", shader_clockings)
shader_surface_data = ClockingData("Shader Surface", "shader_surface", shader_surface_clockings)
shader_surface_eval_data = ClockingData("Shader Surface Eval", "shader_surface_eval", shader_surface_eval_clockings)
shader_surface_sample_data = ClockingData("Shader Surface Sample", "shader_surface_sample", shader_surface_sample_clockings)
shader_surface_preprocess_data = ClockingData("Shader Surface Pre-process", "shader_surface_pre_process", shader_surface_preprocess_clockings)
shader_media_data = ClockingData("Shader Media", "shader_media", shader_media_clockings)
shader_media_eval_data = ClockingData("Shader Media Eval", "shader_media_eval", shader_media_eval_clockings)
shader_media_sample_data = ClockingData("Shader Media Sample", "shader_media_sample", shader_media_sample_clockings)
shader_media_preprocess_data = ClockingData("Shader Media Pre-process", "shader_media_pre_process", shader_media_preprocess_clockings)
filter_data = ClockingData("Filter", "filter", filter_clockings)
embree_data = ClockingData("Embree", "embree", embree_clockings)
debug_one_data = ClockingData("Debug One", "debug_one", debug_one_clockings)
debug_two_data = ClockingData("Debug Two", "debug_two", debug_two_clockings)
debug_three_data = ClockingData("Debug Three", "debug_three", debug_three_clockings)

def create_clocking_graph(data):
    path_to_results = datapath + sys.argv[1] + "/" + sys.argv[1] + "_" + data.save_name + ".png"
    plt.title(data.title)
    plt.xlabel("Iterations")
    plt.ylabel("Time (s)")
    plt.plot(iters, data.data)
    plt.tight_layout()
    plt.savefig(path_to_results, bbox_inches='tight', pad_inches=0)
    plt.clf()

setup()

create_clocking_graph(api_data)
create_clocking_graph(rendering_clockings)
create_clocking_graph(camera_ray_data)
create_clocking_graph(scene_parse_data)
create_clocking_graph(integrator_data)
create_clocking_graph(integrator_intersect_data)
create_clocking_graph(integrator_preprocess_data)
create_clocking_graph(integrator_rr_data)
create_clocking_graph(integrator_nee_data)
create_clocking_graph(integrator_eval_data)
create_clocking_graph(shader_data)
create_clocking_graph(shader_surface_data)
create_clocking_graph(shader_surface_eval_data)
create_clocking_graph(shader_surface_sample_data)
create_clocking_graph(shader_surface_preprocess_data)
create_clocking_graph(shader_media_data)
create_clocking_graph(shader_media_eval_data)
create_clocking_graph(shader_media_sample_data)
create_clocking_graph(shader_media_preprocess_data)
create_clocking_graph(filter_data)
create_clocking_graph(embree_data)
create_clocking_graph(debug_one_data)
create_clocking_graph(debug_two_data)
create_clocking_graph(debug_three_data)

# Label line with line2D label data


# def labelLine(line, x, label=None, align=True, **kwargs):

#     ax = line.axes
#     xdata = line.get_xdata()
#     ydata = line.get_ydata()

#     if (x < xdata[0]) or (x > xdata[-1]):
#         print('x label location is outside data range!')
#         return

#     # Find corresponding y co-ordinate and angle of the line
#     ip = 1
#     for i in range(len(xdata)):
#         if x < xdata[i]:
#             ip = i
#             break

#     y = ydata[ip-1] + (ydata[ip]-ydata[ip-1]) * \
#         (x-xdata[ip-1])/(xdata[ip]-xdata[ip-1])

#     if not label:
#         label = line.get_label()

#     if align:
#         # Compute the slope
#         dx = xdata[ip] - xdata[ip-1]
#         dy = ydata[ip] - ydata[ip-1]
#         ang = degrees(atan2(dy, dx))

#         # Transform to screen co-ordinates
#         pt = np.array([x, y]).reshape((1, 2))
#         trans_angle = ax.transData.transform_angles(np.array((ang,)), pt)[0]

#     else:
#         trans_angle = 0

#     # Set a bunch of keyword arguments
#     if 'color' not in kwargs:
#         kwargs['color'] = line.get_color()

#     if ('horizontalalignment' not in kwargs) and ('ha' not in kwargs):
#         kwargs['ha'] = 'center'

#     if ('verticalalignment' not in kwargs) and ('va' not in kwargs):
#         kwargs['va'] = 'center'

#     if 'backgroundcolor' not in kwargs:
#         kwargs['backgroundcolor'] = ax.get_facecolor()

#     if 'clip_on' not in kwargs:
#         kwargs['clip_on'] = True

#     if 'zorder' not in kwargs:
#         kwargs['zorder'] = 2.5

#     ax.text(x, y, label, rotation=trans_angle, **kwargs)


# def labelLines(lines, align=True, xvals=None, **kwargs):

#     ax = lines[0].axes
#     labLines = []
#     labels = []

#     # Take only the lines which have labels other than the default ones
#     index = 0
#     for line in lines:
#         index = index + 1
#         if (index > 6):  # hardcoded
#             label = line.get_label()
#             if "_line" not in label:
#                 labLines.append(line)
#                 labels.append(label)

#     if xvals is None:
#         xmin, xmax = ax.get_xlim()
#         xvals = np.linspace(xmin, xmax, len(labLines)+2)[1:-1]
#     # hardcoded because linspace does not apply to loglog plots
#     xvals = [16384*8-1, 16384*8-1, 16384*8-1, 16384*8-1, 16384*8-1]

#     for line, x, label in zip(labLines, xvals, labels):
#         labelLine(line, x, label, align, **kwargs)

# # This is meant to be ran from within the same folder
# # TODO: remove this as a script eventually

# # tests = ["0_20", "0_25", "0_30", "0_35", "0_40", "0_45", "0_50", "0_55",
# #          "0_60", "0_65", "0_70", "0_75", "0_80", "0_85", "0_90", "0_95"]


# # tests = [1, 2, 4, 8, 16]
# samples = [1, 2, 4, 8, 16, 32, 64, 128, 256,
#          512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 131072*2, 131072*4, 131072*8]

# # path = "python_ci"
# # path = "power_law_ci_batch"

# # percentiles = [10, 90]

# conv_lines = True

# # samples = []
# final_errors = []

# # alpha_inds = [15, 25, 35, 45, 55, 65]
# # percentile = 10
# # alpha_labels = ["alpha = 0.35", "alpha = 0.45", "alpha = 0.55",
# #                 "alpha = 0.65", "alpha = 0.75", "alpha = 0.85"]

# # for alpha_ind in alpha_inds:
# #     alpha = 0.2 + float(alpha_ind) * 0.01
# #     # first do the percentiles individually

# #     samples = []
# #     errors = []

# #     # graph_title = str(percentile) + " percentile : alpha = " + \
# #     #     str(alpha)
# #     # xaxis_title = "Samples"
# #     # yaxis_title = "Error"

# #     # plt.title(graph_title)
# #     # plt.xlabel(xaxis_title)
# #     # plt.ylabel(yaxis_title)

# #     # plt.grid()

# #     # plt.xscale("log", basex=2)
# #     # plt.yscale("log", basey=2)

# #     # plt.xlim(32, 262144)
# path_to_results = "../../pbrttest/results/bias_conv/bias_vs_var.png"

# #     # path_to_results = "../results/"+path+"/alpha_" + \
# #     #     str(alpha_ind) + "/percentile_err_"+str(percentile)+".png"
# #     # path_to_results = "/Users/corneria/Desktop/"+path+"/alpha_" + \
# #     #     str(alpha_ind) + "/percentile_err_"+str(percentile)+".png"

# #     cnt = 0
# #     for ind in tests:

# #         # is_x_log = False
# #         # is_y_log = False

# #         values = []

# #         data_avg = open(
# #             "../results/"+path+"/alpha_" + str(alpha_ind) + "/" + str(ind) + "_samples.txt").readlines()
# #         siz = int(len(data_avg))
# #         for k in range(0, siz):
# #             values.append(float(data_avg[k]))

# #         values.sort()

# #         perc_ind = int(float(len(values)) * float(percentile) / 100.0)
# #         # print(perc_ind)
# #         # print(math.fabs(values[perc_ind]))

# #         if cnt > 8:
# #             samples.append(ind)
# #             errors.append(math.fabs(values[perc_ind]))
# #         cnt = cnt + 1

#     # final_errors.append(errors)

# graph_title = "Bias Conv, vs. Std. Dev conv (Theoretical)"
# xaxis_title = "Samples"
# yaxis_title = "Measure"

# plt.title(graph_title)
# plt.xlabel(xaxis_title)
# plt.ylabel(yaxis_title)

# plt.grid()

# plt.xscale("log", base=2)
# plt.yscale("log", base=2)

# plt.xlim(1, 262144)

# # path_to_results = "../results/"+path+"/alpha_" + \
# #     str(alpha_ind) + "/percentile_err_"+str(percentile)+".png"
# # path_to_results = "/Users/corneria/Desktop/" + \
# #     path+"/percentile_err_"+str(percentile)+".png"

# # for index in range(0, len(final_errors)):
# #     tmp_start_y = final_errors[index][0]
# #     transl = final_errors[0][0] - tmp_start_y
# #     for index_2 in range(0, len(final_errors[index])):
# #         # final_errors[index][index_2] = final_errors[index][index_2] + transl
# #         final_errors[index][index_2] = final_errors[index][index_2] / tmp_start_y

# index = 0
# # for errors in final_errors:
# #     # print(index)
# #     plt.plot(samples, errors, "-", zorder=3.0, label=alpha_labels[index])
# #     index = index + 1

# # plt.legend(loc=3)

# if conv_lines:
#     start_y = 100
#     start_x = samples[0]

#     one_slope = [start_y]
#     two_slope = [start_y]
#     three_slope = [start_y]
#     four_slope = [start_y]
#     five_slope = [start_y]
#     six_slope = [start_y]

#     for i in range(1, len(samples)):
#         one_slope.append(
#             one_slope[0] / math.pow(start_x, -0.1) * math.pow(samples[i], -0.1))
#         two_slope.append(
#             one_slope[0] / math.pow(start_x, -0.2) * math.pow(samples[i], -0.2))
#         three_slope.append(
#             two_slope[0] / math.pow(start_x, -0.3) * math.pow(samples[i], -0.3))
#         four_slope.append(
#             four_slope[0] / math.pow(start_x, -0.4) * math.pow(samples[i], -0.4))
#         five_slope.append(
#             four_slope[0] / math.pow(start_x, -0.5) * math.pow(samples[i], -0.5))
#         six_slope.append(six_slope[0] / samples[i])

#     # plt.plot(samples, one_slope, "--k", alpha=0.2, label="-0.1")
#     # plt.plot(samples, two_slope, "--k", alpha=0.2, label="-0.2")
#     # plt.plot(samples, three_slope, "--k", alpha=0.2, label="-0.3")
#     # plt.plot(samples, four_slope, "--k", alpha=0.2, label="-0.4")
#     plt.plot(samples, five_slope, "-k", color="blue", alpha=1.0, label="std. dev.")
#     plt.plot(samples, six_slope, "-k", color="red", alpha=1.0, label="bias")

#     # labelLines(plt.gca().get_lines(), zorder=2.5, alpha=0.2)

# plt.legend(loc=3)

# # plt.plot(samples, errors, "r-")
# plt.savefig(path_to_results)
# plt.clf()