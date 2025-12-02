import csv
import sys
from datetime import datetime
import matplotlib.pyplot as plt

def extract_fft_data_from_log(log_path):
    header_str = "Bin,Frequency_Hz,Power"
    end_marker = "=== AUDIO FFT done ==="

    data_rows = []
    header_found = False

    with open(log_path, "r", encoding="utf-8", errors="replace") as f:
        for line in f:
            clean_line = line.strip()

            if not header_found:
                if clean_line.startswith(header_str):
                    header_found = True
                    data_rows.append(header_str.split(","))
                continue

            if header_found:
                if clean_line.startswith(end_marker):
                    break

                if not clean_line:
                    continue

                parts = clean_line.split(",")
                if len(parts) >= 3:
                    data_rows.append(parts[:3])

    return data_rows

def save_csv(data_rows):
    if not data_rows:
        print("No data found to save.")
        return None

    timestamp = datetime.now().strftime("%y%m%d_%H%M%S")
    filename = f"Project6_{timestamp}.csv"

    with open(filename, "w", newline="", encoding="utf-8") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerows(data_rows)

    print(f"CSV saved as: {filename}")
    return filename

def plot_fft_from_rows(data_rows):
    if not data_rows or len(data_rows) < 2:
        print("Not enough data to plot.")
        return

    header = data_rows[0]
    data = data_rows[1:]

    try:
        freq_idx = header.index("Frequency_Hz")
        power_idx = header.index("Power")
    except ValueError:
        print("Header does not contain expected columns.")
        return

    freqs = []
    powers = []

    for row in data:
        if len(row) <= max(freq_idx, power_idx):
            continue
        try:
            freq = float(row[freq_idx])
            power = float(row[power_idx])
            freqs.append(freq)
            powers.append(power)
        except ValueError:
            continue

    if not freqs or not powers:
        print("Could not extract numeric data to plot.")
        return

    plt.figure(figsize=(10, 5))
    markerline, stemlines, baseline = plt.stem(freqs, powers)
    baseline.set_visible(False)
    plt.setp(stemlines, linewidth=1)
    plt.setp(markerline, markersize=4)
    plt.xlabel("Frequency (Hz)")
    plt.ylabel("Power")
    plt.title("FFT - Frequency vs Power (Stem Plot)")
    plt.grid(True)
    plt.tight_layout()

def extract_codec_data_from_log(log_path):
    start_marker = "=== CODEC audio captured start ==="
    end_marker   = "=== CODEC audio captured done ==="

    started = False
    indices = []
    values = []

    with open(log_path, "r", encoding="utf-8", errors="replace") as f:
        for line in f:
            clean_line = line.strip()

            if not started:
                if clean_line.startswith(start_marker):
                    started = True
                continue

            if clean_line.startswith(end_marker):
                break

            if not clean_line:
                continue

            if "AUDIO_DATA" in clean_line and "=" in clean_line:
                try:
                    left, right = clean_line.split("=")
                    right = right.strip()
                    left = left.strip()
                    idx_start = left.find("[") + 1
                    idx_end = left.find("]")
                    idx_str = left[idx_start:idx_end]
                    idx = int(idx_str)
                    val = int(right)
                    indices.append(idx)
                    values.append(val)
                except Exception:
                    continue

    return indices, values

def plot_codec_samples(indices, values):
    if not values:
        print("No codec samples found to plot.")
        return

    plt.figure(figsize=(10, 4))
    plt.plot(indices, values, marker="o", linestyle="-")
    plt.xlabel("Sample index")
    plt.ylabel("Amplitude")
    plt.title("Codec captured audio (time domain)")
    plt.grid(True)
    plt.tight_layout()

def main():
    if len(sys.argv) < 2:
        print("Usage: python extract_fft_log.py <log_path>")
        sys.exit(1)

    log_path = sys.argv[1]

    print(f"Reading log: {log_path}")

    codec_indices, codec_values = extract_codec_data_from_log(log_path)
    if not codec_values:
        print("No codec audio capture block found in log.")
    else:
        plot_codec_samples(codec_indices, codec_values)

    data_rows = extract_fft_data_from_log(log_path)
    if not data_rows:
        print("No FFT blocks found in log.")
    else:
        csv_filename = save_csv(data_rows)
        plot_fft_from_rows(data_rows)

    plt.show()

if __name__ == "__main__":
    main()
