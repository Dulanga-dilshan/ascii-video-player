# ASCII Video Player (C)

A terminal-based media processing project that evolved from BMP/MP4 decoding to real-time ASCII video rendering via ffmpeg pipes.

## Project Evolution
This project represents my journey in low-level media processing:

1. **Phase 1: BMP Image Decoder**  
   - Implemented BMP header parsing and pixel manipulation
   - Learned about file formats and memory-mapped I/O

2. **Phase 2: Video Frame Processing**  
   - Extended to MP4 video frame extraction
   - Explored frame-by-frame decoding

3. **Phase 3: Real-time Streaming**  
   - Integrated ffmpeg pipe-based streaming
   - Implemented real-time frame processing without intermediate files

4. **Phase 4: ASCII Rendering**  
   - Added terminal-based ASCII art output
   - Optimized for real-time video playback

## Features
- BMP image decoding and processing
- Video stream processing via ffmpeg pipes
- Real-time ASCII rendering for terminal display
- Modular architecture for easy extension
