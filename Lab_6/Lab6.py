import subprocess

def add_watermark(input_video, watermark_image, output_video, position='center'):
    if position == 'top_left':
        position_args = "overlay=10:10"
    elif position == 'top_right':
        position_args = "overlay=main_w-overlay_w-10:10"
    elif position == 'bottom_left':
        position_args = "overlay=10:main_h-overlay_h-10"
    elif position == 'bottom_right':
        position_args = "overlay=main_w-overlay_w-10:main_h-overlay_h-10"
    else:
        position_args = "overlay=(main_w-overlay_w)/2:(main_h-overlay_h)/2"
    command = [
        'ffmpeg',
        '-i', input_video,
        '-i', watermark_image,
        '-filter_complex', position_args,
        '-codec:a', 'copy',
        output_video
    ]
    subprocess.run(command)

input_video = 'video.mp4'
watermark_image = 'watermark.png'
output_video = 'output.mp4'
position = 'bottom_right'

add_watermark(input_video, watermark_image, output_video, position)
