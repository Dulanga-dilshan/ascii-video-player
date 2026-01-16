# ASCII Video Player (C)

A terminal-based ASCII video player written in C that streams video frames via ffmpeg pipes and renders them as ASCII characters in real time.

## Project Evolution

1. **Phase 1: BMP Image Decoder**  
   - Implemented BMP header parsing and pixel manipulation
   - Learned about file formats and memory-mapped I/O

2. **Phase 2: Video Frame Processing**  
   - Extended to MP4 video frame extraction
   - Explored frame-by-frame decoding

3. **Phase 3: Real-time Streaming via ffmpeg pipe**  
   - Integrated ffmpeg pipe-based streaming
   - Read raw frames directly from ffmpeg stdout
   - Avoided intermediate files, enabling true real-time processing

4. **Phase 4: ASCII Rendering**  
   - Added terminal-based ASCII art output
   - Optimized for real-time video playback


## Features
- BMP image decoding and processing
- Video stream processing via ffmpeg pipes
- Real-time ASCII rendering for terminal display
- Modular architecture for easy extension


## Tech Stack & Requirements

- Language: C (C11)
- Platform: Linux / Unix-like terminals
- Dependencies: ffmpeg (must be installed on system)
- Terminal: ANSI-compatible terminal for ASCII rendering
- Build: GCC + Makefile


## Example Usage

# Make sure you have ffmpeg installed and a terminal-compatible video
./main input.mp4

# Internally, frames are streamed using ffmpeg via a pipe:
# ffmpeg -i input.mp4 -f rawvideo -pix_fmt gray -
