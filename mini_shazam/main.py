import numpy as np
import scipy
from scipy import signal
import os
import wave
from matplotlib import pyplot as plt

def findFeatures(file, trim=False): # izvlacenje specificnih karakteristika pesme

    # ucitavanje pesme
    wav_obj = wave.open(file, 'rb')
    Fs = wav_obj.getframerate() # odabirci po sekundi
    n_samples = wav_obj.getnframes()

    signal_wave = wav_obj.readframes(n_samples)
    song = np.frombuffer(signal_wave, dtype=np.int16)

    window_length_seconds = 1
    window_length_samples = int(window_length_seconds * Fs)
    window_length_samples += window_length_samples % 2

    # furijeova transformacija
    frequencies, times, stft = signal.stft(
        song, Fs, nperseg=window_length_samples,
        nfft=window_length_samples, return_onesided=True
    )
    num_peaks = 10
    fmax = 4000 # u pesmama uglavnom varira do ove frekvencije
    freq_slice = np.where((frequencies >= 0) & (frequencies <= fmax))
    frequencies = frequencies[freq_slice]
    stft = stft[freq_slice,:][0]
    constellation_map = []
    for time_idx, window in enumerate(stft.T):
        if(trim and time_idx >= 20): break # da ne racuna sve za odsecke
        spectrum = abs(window) # posto je sprektum kompleksan
        peaks, props = signal.find_peaks(spectrum, prominence=0, distance=200)
        # izabrao sam 10 pikova po vremenskom odabiru
        n_peaks = min(num_peaks, len(peaks))
        if(n_peaks != 10): continue
        largest_peaks = np.argpartition(props["prominences"], -n_peaks)[-n_peaks:]
        for peak in peaks[largest_peaks]:
            frequency = frequencies[peak]
            constellation_map.append([time_idx, frequency])

    features = []
    for i in range (0,len(constellation_map),num_peaks):
        collumn = []
        for j in range(num_peaks):
            f = constellation_map[i+j][1]
            # zaokruzivanje vrednosti frekvencija
            mod25 = f % 25
            mod50 = f % 50
            if(mod50 >= 25): collumn.append(int(f+25-mod25))
            else: collumn.append(int(f-mod25))
        features.append(sorted(collumn))
    print("Ucitano: " + file)
    return features


def Matching(mat, submat): # prolazak da li je isecak priblizno jednak nekom delu pesme
    m_len = len(mat)
    sm_len = len(submat)
    cntmax = 0
    for i in range (m_len-sm_len):
        cnt = 0
        for si in range (sm_len):
            for j in range (10):
                if (abs(mat[i+si][j]- submat[si][j]) <=  50):
                    cnt = cnt + 1
        if (cnt > cntmax): cntmax = cnt

    return cntmax

def main():  # glavni main, Shazam

    # pesme bi trebalo da budu u wav formatu
    # konverzija iz mp3 u wav je usporavala program pa zato nisam stavio

    directory_path = 'data'

    # isecak bi trebalo da bude duzi od 5s
    #recording_path = 'dataCut/cut(EKV Zemlja).wav'
    #recording_path = 'dataCut/cut(Crazy on you).wav'
    recording_path = 'dataCut/wrongCut.wav'    # nema pesme u bazi

    songs = []
    for filename in os.listdir(directory_path):
        if filename.endswith('.wav') :
            song_path = os.path.join(directory_path, filename)
            song_features = findFeatures(song_path)
            songs.append({'path': song_path, 'features': song_features, 'matches': 0})

    print("\nIsecak")
    recording_features = findFeatures(recording_path,True)

    max_matches = 0
    final_song = ""
    for song in songs:
        matches = Matching(song['features'],recording_features)
        if (matches > max_matches):
            max_matches = matches
            final_song = song['path']

    if ( max_matches > 100) : print("\nDobijena pesma: " + final_song)
    else: print("No matches")
    # print(max_matches) max_matches varira od 0 do 200 => u proseku je preko 130 ako ima pesme u bazi
    # ukoliko postoji pesma, a ipak je ne prepoznaje => treba smanjiti kriterijum za max_matches

def main2():   # Spektogram pesme
    # ucitavanje pesme
    file = "data/EKV_ZEMLJA.wav"
    wav_obj = wave.open(file, 'rb')
    Fs = wav_obj.getframerate()  # odabirci po sekundi
    n_samples = wav_obj.getnframes()

    signal_wave = wav_obj.readframes(n_samples)
    song = np.frombuffer(signal_wave, dtype=np.int16)

    window_length_seconds = 1
    window_length_samples = int(window_length_seconds * Fs)
    window_length_samples += window_length_samples % 2

    # furijeova transformacija
    frequencies, times, stft = signal.stft(
        song, Fs, nperseg=window_length_samples,
        nfft=window_length_samples, return_onesided=True
    )
    num_peaks = 10
    fmax = 4000  # u pesmama uglavnom varira do ove frekvencije
    freq_slice = np.where((frequencies >= 0) & (frequencies <= fmax))
    frequencies = frequencies[freq_slice]
    stft = stft[freq_slice, :][0]

    plt.title('Spectrogram')
    Pxx, freqs, bins, im = plt.specgram(song, Fs=Fs, NFFT=512)
    plt.xlabel('Time')
    plt.ylabel('Frequency')
    plt.xlim(left=0, right=20)
    plt.ylim((0,4000))

    plt.show()

def main3():  #  Kako izleda features pesme
    file = "data/EKV_ZEMLJA.wav"

    m = findFeatures(file)

    for i in range(len(m[0])):
        x = []
        y = []
        for j in range(0,len(m),4):
            x.append(j/4)
            y.append(m[j][i])
        plt.scatter(x,y,color='blue')

    plt.xlim(left=0,right=40)
    plt.xlabel("time(s)")
    plt.ylabel("frequency(Hz)")
    plt.show()

def main4():  # spektar frekvencije, furijeova transformacija
    file = "data/Crazy On You (feat. Heart).wav"
    wav_obj = wave.open(file, 'rb')
    Fs = wav_obj.getframerate()  # odabirci po sekundi
    n_samples = wav_obj.getnframes()

    signal_wave = wav_obj.readframes(n_samples)
    song = np.frombuffer(signal_wave, dtype=np.int16)

    N = len(song)
    fft = scipy.fft.fft(song)
    transform_y = 2.0 / N * np.abs(fft[0:N // 2])
    transform_x = scipy.fft.fftfreq(N, 1 / Fs)[:N // 2]
    plt.plot(transform_x, transform_y)
    plt.xlabel("Frequency (Hz)");

    plt.xlim(0, 4000);
    plt.show()

if __name__ == '__main__':
    main()


