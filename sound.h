// All sound manipulation here
#include "fmod.h"


CNukeFile		SD;

////////////////////////////////////////////////////////////////////////////////////////////
// Name of Game Sound Resource File														  //
const LPSTR sndnf="SOUND.DAT";															  //
////////////////////////////////////////////////////////////////////////////////////////////

BOOL snd;

FSOUND_STREAM *stream;
FSOUND_STREAM *click;
FSOUND_STREAM *next;
FSOUND_STREAM *sample;
FSOUND_STREAM *sound;
FSOUND_SAMPLE *samp;
int channel;
int chnl;

BOOL InitSound(HWND hWnd)
{
	snd=true;
	FSOUND_SetHWND(hWnd);
	FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND);
	if(!FSOUND_Init(44100, 16, 0))
	{
		ShowCursor(true);
		MessageBox(hWnd, "Can't init sound.", "Error", MB_OK);
		ShowCursor(false);
		snd=false;
	}

	return TRUE;
}

bool OpenSndDataFile()
{
	SD.SetFileDescription("Fallout Data File");
	SD.Open(sndnf);

	return SD.IsFileOpen();
}

void OpenMStream(CString file)
{
	if(snd)
	{
		LPSTR p = file.GetBuffer(file.GetLength());
		file.ReleaseBuffer( );
	
		stream=FSOUND_Stream_OpenFile(p, FSOUND_LOOP_NORMAL | FSOUND_STEREO | FSOUND_16BITS, 0);
	}
}

void OpenSStream(CString file)
{
	if(snd)
	{
		if(OpenSndDataFile())
		{
			LPSTR p = file.GetBuffer(file.GetLength());
			file.ReleaseBuffer( );

			NF_FileBuffer buffer;
			buffer = SD.GetFile(p);

			sound=FSOUND_Stream_OpenFile((const char*)buffer.FileBuffer, FSOUND_LOADMEMORY | FSOUND_NORMAL, buffer.Size);
			int chn = FSOUND_Stream_Play(FSOUND_FREE, sound);
			FSOUND_SetVolume(chn, FSOUND_GetSFXMasterVolume());

			SD.Close();
		}
	}
}

void Open3DSSound(CString file, float pos[3])
{
	if(snd)
	{
		if(OpenSndDataFile())
		{
			LPSTR p = file.GetBuffer(file.GetLength());
			file.ReleaseBuffer( );

			NF_FileBuffer buffer;
			buffer = SD.GetFile(p);

			sound=FSOUND_Stream_OpenFile((const char*)buffer.FileBuffer, FSOUND_LOADMEMORY | FSOUND_HW3D, buffer.Size);
			if(sound==NULL)
				sound=FSOUND_Stream_OpenFile((const char*)buffer.FileBuffer, FSOUND_LOADMEMORY | FSOUND_NORMAL, buffer.Size);
			int chn = FSOUND_Stream_Play3DAttrib(FSOUND_FREE, sound, -1, -1, -1, pos, NULL);
			FSOUND_Reverb_SetMix(chn, FSOUND_REVERBMIX_USEDISTANCE);

			SD.Close();
		}
	}
}

void Open3DSSoundLoop(CString file, float pos[3])
{
	if(snd)
	{
		if(OpenSndDataFile())
		{
			LPSTR p = file.GetBuffer(file.GetLength());
			file.ReleaseBuffer( );

			NF_FileBuffer buffer;
			buffer = SD.GetFile(p);

			sound=FSOUND_Stream_OpenFile((const char*)buffer.FileBuffer, FSOUND_LOADMEMORY | FSOUND_HW3D | FSOUND_LOOP_NORMAL, buffer.Size);
			if(sound==NULL)
				sound=FSOUND_Stream_OpenFile((const char*)buffer.FileBuffer, FSOUND_LOADMEMORY | FSOUND_LOOP_NORMAL, buffer.Size);
			chnl = FSOUND_Stream_Play3DAttrib(FSOUND_FREE, sound, -1, -1, -1, pos, NULL);
			FSOUND_Reverb_SetMix(chnl, FSOUND_REVERBMIX_USEDISTANCE);

			SD.Close();
		}
	}
}

void OpenClickStream()
{
	if(snd)
	{
		if(OpenSndDataFile())
		{
			NF_FileBuffer buffer;
			buffer = SD.GetFile("CLICK.WAV");

			click=FSOUND_Stream_OpenFile((const char*)buffer.FileBuffer, FSOUND_LOADMEMORY | FSOUND_NORMAL, buffer.Size);

			SD.Close();
		}
//		click=FSOUND_Stream_OpenFile("data\\sound\\CLICK.WAV", FSOUND_NORMAL, 0);
	}
}

void OpenNextSStream()
{
	if(snd)
	{
		if(OpenSndDataFile())
		{
			NF_FileBuffer buffer;
			buffer = SD.GetFile("NEXT.WAV");

			next=FSOUND_Stream_OpenFile((const char*)buffer.FileBuffer, FSOUND_LOADMEMORY | FSOUND_NORMAL, buffer.Size);

			SD.Close();
		}
//		next=FSOUND_Stream_OpenFile("data\\sound\\NEXT.WAV", FSOUND_NORMAL, 0);
	}
}

void Click()
{
	if(snd)
	{
		int chn = FSOUND_Stream_Play(FSOUND_FREE, click);
		FSOUND_SetVolume(chn, FSOUND_GetSFXMasterVolume());
	}
}

void NextS()
{
	if(snd)
	{
		int chn = FSOUND_Stream_Play(FSOUND_FREE, next);
		FSOUND_SetVolume(chn, FSOUND_GetSFXMasterVolume());
	}
}

void PlayMStream()
{
	if(snd)
	{
		channel = FSOUND_Stream_Play(FSOUND_FREE, stream);
		FSOUND_SetPan(channel, FSOUND_STEREOPAN);
	}
}

void StopMStream()
{
	if(snd)
	{
		FSOUND_Stream_Stop(stream);
	}
}

void PauseMStream(bool pause)
{
	if(snd)
	{
		if(pause)
			FSOUND_Stream_SetPaused(stream, FALSE);
		else
			FSOUND_Stream_SetPaused(stream, TRUE);
	}
}

void DeleteSound()
{
	if(snd)
	{
		FSOUND_Stream_Close(stream);
		FSOUND_Stream_Close(click);
		FSOUND_Stream_Close(next);
		FSOUND_Stream_Close(sample);
		FSOUND_Stream_Close(sound);
		FSOUND_Close();
	}
}

void SetMVolume(int vol)
{
	if(snd)
	{
		channel = FSOUND_Stream_Play(FSOUND_FREE, stream);
		FSOUND_SetVolume(channel, vol);
	}
}

int GetMVolume()
{
	int r=0;
	if(snd)
	{
		r = FSOUND_GetVolume(channel);
	}

	return r;
}

void SetMPosition()
{
	if(snd)
	{
		int l = FSOUND_Stream_GetLengthMs(stream);

		l = l/2;

		int pos = rand() % l;

		FSOUND_Stream_SetTime(stream, pos);
	}
}

void PlaySample(CString file)
{
	if(snd)
	{
		LPSTR p = file.GetBuffer(6);
		file.ReleaseBuffer( );
	
		sample=FSOUND_Stream_OpenFile(p, FSOUND_NORMAL, 0);

		int chn = FSOUND_Stream_Play(FSOUND_FREE, sample);
		FSOUND_SetVolume(chn, FSOUND_GetSFXMasterVolume());
	}
}

void MuteSound(BOOL mute)
{
	if(snd)
	{
		FSOUND_SetMute(FSOUND_ALL, mute);
		FSOUND_SetMute(channel, mute);
	}
}

void SetVolume(int volume)
{
	if(snd)
	{
		int vol = volume * 2.55;
		FSOUND_SetSFXMasterVolume(vol);
	}
}

void StopSStream()
{
	if(snd)
	{
		FSOUND_Stream_Stop(sound);
		FSOUND_StopSound(chnl);
	}
}

void PauseSStream()
{
	if(snd)
	{
		if(FSOUND_Stream_GetPaused(sound))
			FSOUND_Stream_SetPaused(sound, FALSE);
		else
			FSOUND_Stream_SetPaused(sound, TRUE);
	}
}


