"""Display charts from moci profiling files. Chrome tracing format used.
"""
import json

import numpy as np
import matplotlib.pyplot as plt


def read_profile_json(path):
    with open(path, "r") as read_file:
        return json.load(read_file)


def top_level_groups(data):
    groups = []
    for key, _ in data.items():
        groups.append(key)
    return groups


def num_trace_events(data):
    """Returns the total number of traces captured
    """
    return len(data["traceEvents"])


def num_unique_trace_events(data):
    """Returns the total number of unique traces captured
    """
    unique_events = {}
    for event in data["traceEvents"]:
        if "name" in event:
            unique_events[event["name"]] = 1
    return len(unique_events)


def group_by_event(data):
    """Returns a dict with lists of floats. Key is the trace name.
    """
    average_collector = {}
    for event in data["traceEvents"]:
        if "name" in event:
            if event["name"] in average_collector:
                average_collector[event["name"]].append(event["dur"])
            else:
                average_collector[event["name"]] = [event["dur"]]
    return average_collector


def average_for_each_event(data):
    """Returns a dict with min, max & average for each unique trace event.
    """
    averages = {}
    for key, event in data.items():
        total = 0.0
        minimum = 999999999999.0
        maximum = 0.0
        if len(event) >= 2:
            event.remove(max(event))
            event.remove(min(event))
        for trace in event:
            total += trace
            if trace < minimum:
                minimum = trace
            if trace > maximum:
                maximum = trace
        if len(event) > 0:
            averages[key] = {"average": total / len(event),
                             "min": minimum,
                             "max": maximum}
    return averages


def show_averages(data):
    n_groups = len(data)
    means_min = []
    means_max = []
    means_average = []

    x_values = []
    for key, average in data.items():
        x_values.append(key)
        means_min.append(average["min"])
        means_max.append(average["max"])
        means_average.append(average["average"])

    # create plot
    _, _ = plt.subplots()
    index = np.arange(n_groups)
    bar_width = 0.2
    opacity = 0.9

    _ = plt.bar(index, means_min, bar_width,
                alpha=opacity,
                color='g',
                label='Min')

    _ = plt.bar(index + bar_width, means_max, bar_width,
                alpha=opacity,
                color='b',
                label='Average')

    _ = plt.bar(index + (bar_width*2), means_average, bar_width,
                alpha=opacity,
                color='r',
                label='Max')

    plt.xlabel('Trace')
    plt.ylabel('Microseconds')
    plt.title('Summary')

    plt.xticks(index + bar_width, x_values)
    plt.legend()

    plt.tight_layout()
    plt.show()


def show_range(data):
    data.remove(max(data))
    data.remove(min(data))
    performance = []
    for trace in data:
        performance.append(trace)

    y_pos = np.arange(len(performance))
    plt.plot(y_pos, performance)
    plt.ylabel('Microseconds')
    plt.title('Range')
    plt.show()

def show_standard_deviation(data):
    data.remove(max(data))
    data.remove(min(data))
    performance = []
    for trace in data:
        performance.append(trace)

    deviation = int(np.std(performance))
    print(f"Standard deviation: {deviation} micro seconds") 

    # y_pos = np.arange(len(performance))
    # plt.plot(y_pos, performance)
    # plt.ylabel('Microseconds')
    # plt.title('Range')
    # plt.show()

def main():
    profile = read_profile_json("moci-sandbox-3d.json")
    print(top_level_groups(profile))
    print(num_trace_events(profile))
    print(num_unique_trace_events(profile))

    grouped = group_by_event(profile)
    # show_range(grouped['virtual void DemoLayer::OnUpdate(moci::Timestep)'])
    show_standard_deviation(grouped['virtual void DemoLayer::OnUpdate(moci::Timestep)'])

    # averages = average_for_each_event(grouped)
    # show_averages(averages)


if __name__ == "__main__":
    main()
