package cn.cloudwalk.camera;

import java.io.Closeable;
import java.nio.ByteBuffer;
import java.nio.channels.ClosedByInterruptException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import android.R.integer;
import android.media.MediaCodec;
import android.media.MediaCodec.CryptoException;
import android.media.MediaFormat;
import android.util.Log;

public class DecodeStream {
	
	private static DecodeStream api = null;
	
	private static boolean closed = false;
	static private RtspObserver observer=null;
	private static Thread _thread; 
	private static VideoDecoder _decoderDecoder;
	private static MediaInfo Information;
	private static List<byte[]> mylist = new ArrayList<byte[]>();
	private static byte[] i_frame = null;
	public void setCallbak(RtspObserver obs)
	{
		observer = obs;
	}
	private static void stopThread()
	{
		closed = true;
	}
	private static void decode_queue_packet()
	{
		while(mylist.size() > 0)
		{
			byte[] cur = mylist.get(0);
			//wait 100ms for input buffer
			if(_decoderDecoder.writeInput(cur,100000) < 0)
			{
				Log.e("Media","writeInput error");
				break;
			}
			mylist.remove(0);
		}
	}
	private static void startThread()
	{
		closed = false;
		
		new Thread(new Runnable() {
			
			@Override
			public void run() {
				while(!closed)
				{
					byte[] outData=_decoderDecoder.getOutput(20000);
					while(outData != null)
					{
						if(observer!=null)
						{
							int size =Information.width * Information.height;
							
							byte[] yarr = Arrays.copyOfRange(outData, 0, size );
		            		byte[] uarr = Arrays.copyOfRange(outData, size, size*5/4);
		            		byte[] varr = Arrays.copyOfRange(outData, size*5/4, size*3/2);
							observer.onDataArrival(yarr, uarr,varr);
						}
						outData=_decoderDecoder.getOutput(0);
					}
				}
			}
		}).start();
		//���ӳɹ��󴴽��߳�.�������ӳɹ����߳̿�ʼ����.ƽʱ�߳���˯�ߵ�.
		_thread = new Thread(new Runnable() {
			
			@Override
			public void run() {
				int count = 0;
				// TODO Auto-generated method stub
				while(!closed)
				{
					int timeout_ms = 500;
					byte arr[] = GetCameraStreamJNI.get().GetImage(timeout_ms);
					
					if(arr==null) 
					{
						decode_queue_packet();
						continue;
					}
					
					mylist.add(arr);
					/*if(count++%100 == 0)
					{
						if(_decoderDecoder.writeInput(i_frame,1000000) < 0)
						{
							Log.e("Media","writeInput error");
						}
					}
					else*/
					decode_queue_packet();
					//����ȡ��һ֡���ݺ��п��ܻ�ȡ����һ֡���Խ����buffer����������д��ʧ�ܣ��൱�ڰ�����ʧ��.	
					
				
					/*byte[] outData=_decoderDecoder.getOutput(20000);
					while(outData != null)
					{
						if(observer!=null)
						{
							int size =Information.width * Information.height;
							
							byte[] yarr = Arrays.copyOfRange(outData, 0, size );
		            		byte[] uarr = Arrays.copyOfRange(outData, size, size*5/4);
		            		byte[] varr = Arrays.copyOfRange(outData, size*5/4, size*3/2);
							observer.onDataArrival(yarr, uarr,varr);
						}
						outData=_decoderDecoder.getOutput(0);
					}*/
				
					
				}
				
				Log.e("Media", "decoder thread exit");
			}
		});
		_thread.start();
	}
	public int Start(String url,int timeout_ms,boolean auto_reconnect,boolean tcp){
		
		int err = GetCameraStreamJNI.get().Open(url,timeout_ms,auto_reconnect,tcp);
		//������ͷ���ó��첽�����������������ӳɹ�����jni��ص�״̬�����ݷ��صĸ�ʽ���ٴ���������
		//���߽��������ŵ�jni��ȥ��.�Զ�����.���ӳɹ����߶Ͽ���ص�֪ͨ�����.
		if(err != 0 ) 
		{
			return err;
		}

		return 0;
	}
	
	public MediaInfo GetInfo(){
		
		return Information;
	}

	
	static public void notifyClosed()
	{
	
	
		if(observer!=null)
		{
			observer.onDisconnect();
		}
		stopThread();
	}
	public int Closeurl()
	{
		closed  = true;
		stopThread();
		int ret = GetCameraStreamJNI.get().Close();
		Log.d("me", "ret="+ret);
		return ret;
	}
	
	public static DecodeStream get()
	{
		if(api == null)
			api = new DecodeStream();
		return api;
	}
	public static int notifyOpened() {
		// TODO Auto-generated method stub
		
		Information = GetCameraStreamJNI.get().GetMediaInfo();
		if(Information==null)
		{
			Log.e("Media","Can not get Information");
			return -1;
		}
		if(_decoderDecoder==null)
		{
			_decoderDecoder = new VideoDecoder();
	    
	    	_decoderDecoder.init(Information);
	    	
		}
		
		if(observer!=null)
		{
			observer.onConnect();
			observer.onConfigConfiged(Information.width, Information.height,Information.format);
			Log.e("Media","width="+Information.width+"height="+Information.height+"format="+Information.format);
		}
		startThread();
		return 0;
	}
	public void pause(boolean b) {
		// TODO Auto-generated method stub
		GetCameraStreamJNI.get().Pause(b);
	}
	
}

