// Copyright (c) 2022 Nineva Studios

package com.ninevastudios.unrealfirebase;

import android.app.Activity;
import android.net.Uri;

import androidx.annotation.NonNull;

import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.firebase.storage.FileDownloadTask;
import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.OnProgressListener;
import com.google.firebase.storage.StorageMetadata;
import com.google.firebase.storage.StorageReference;
import com.google.firebase.storage.UploadTask;

import java.io.File;
import java.io.IOException;
import java.util.HashMap;

@SuppressWarnings("unused")
public class FGCloudStorage {
	private static final FirebaseStorage storage = FirebaseStorage.getInstance();
	private static final StorageReference storageRef = storage.getReference();

	public static native void OnUploadFromLocalFileProgress(long callback, int progress);
	public static native void OnUploadFromLocalFileFailure(long callback, String error);
	public static native void OnUploadFromLocalFileComplete(long callback);

	public static native void OnUploadFromDataInMemoryProgress(long callback, int progress);
	public static native void OnUploadFromDataInMemoryFailure(long callback, String error);
	public static native void OnUploadFromDataInMemoryComplete(long callback);

	public static native void OnDownloadInMemoryComplete(long callback, byte[] bytes);
	public static native void OnDownloadInMemoryError(long callback, String error);

	public static native void OnDownloadToLocalFileProgress(long callback, int progress);
	public static native void OnDownloadToLocalFileComplete(long callback, String path);
	public static native void OnDownloadToLocalFileError(long callback, String error);

	public static native void OnGetMetadataComplete(long callback, StorageMetadata storageMetadata);
	public static native void OnGetMetadataError(long callback, String error);

	public static native void OnUpdateMetadataComplete(long callback, StorageMetadata storageMetadata);
	public static native void OnUpdateMetadataError(long callback, String error);

	public static native void OnDeleteFileComplete(long callback);
	public static native void OnDeleteFileError(long callback, String error);

	public static native void OnGetDownloadUrlComplete(long callback, String downloadUrl);
	public static native void OnGetDownloadUrlError(long callback, String error);

	public static void uploadFromDataInMemory(final long callback, byte[] buffer , String storagePath) {
		StorageReference uploadedRef = storageRef.child(storagePath);

		UploadTask uploadTask = uploadedRef.putBytes(buffer);
		uploadTask.addOnProgressListener(new OnProgressListener<UploadTask.TaskSnapshot>() {
			@Override
			public void onProgress(UploadTask.TaskSnapshot taskSnapshot) {
				float progress = (100.0f * taskSnapshot.getBytesTransferred()) / taskSnapshot.getTotalByteCount();
				OnUploadFromDataInMemoryProgress(callback, (int)progress);
			}
		}).addOnFailureListener(new OnFailureListener() {
			@Override
			public void onFailure(Exception e) {
				OnUploadFromDataInMemoryFailure(callback, e.toString());
			}
		}).addOnSuccessListener(new OnSuccessListener<UploadTask.TaskSnapshot>() {
			@Override
			public void onSuccess(UploadTask.TaskSnapshot taskSnapshot) {
				OnUploadFromDataInMemoryComplete(callback);
			}
		});
	}

	public static void uploadFromLocalFile(final long callback,  String filePath, final String storagePath) {
		final Uri file = Uri.fromFile(new File(filePath));

		final UploadTask uploadTask = storageRef
				.child(storagePath).putFile(file);

		uploadTask.addOnProgressListener(new OnProgressListener<UploadTask.TaskSnapshot>() {
			@Override
			public void onProgress(UploadTask.TaskSnapshot taskSnapshot) {
				float progress = (100.0f * taskSnapshot.getBytesTransferred()) / taskSnapshot.getTotalByteCount();

				OnUploadFromLocalFileProgress(callback, (int)progress);
			}
		}).addOnFailureListener(new OnFailureListener() {
			@Override
			public void onFailure(Exception e) {
				OnUploadFromLocalFileFailure(callback, e.toString());
			}
		}).addOnSuccessListener(new OnSuccessListener<UploadTask.TaskSnapshot>() {
			@Override
			public void onSuccess(UploadTask.TaskSnapshot taskSnapshot) {
				OnUploadFromLocalFileComplete(callback);
			}
		});
	}

	public static void getDownloadUrl(final long callback, String fileStoragePath)
	{
		storageRef.child(fileStoragePath).getDownloadUrl()
				.addOnSuccessListener(new OnSuccessListener<Uri>() {
			@Override
			public void onSuccess(Uri uri) {
				OnGetDownloadUrlComplete(callback, uri.toString());
			}
		}).addOnFailureListener(new OnFailureListener() {
			@Override
			public void onFailure(Exception e) {
				OnGetDownloadUrlError(callback, e.toString());
			}
		});
	}

	public static void downloadInMemory(final long callback, String storageFilePath, long fileSizeLimit) {
		StorageReference fileRef = storageRef.child(storageFilePath);

		fileRef.getBytes(fileSizeLimit).addOnSuccessListener(new OnSuccessListener<byte[]>() {
			@Override
			public void onSuccess(byte[] bytes) {
				OnDownloadInMemoryComplete(callback, bytes);
			}
		}).addOnFailureListener(new OnFailureListener() {
			@Override
			public void onFailure(Exception e) {
				OnDownloadInMemoryError(callback, e.toString());
			}
		});
	}

	public static void downloadToLocalFile(Activity context, final long callback, String storageFilePath, final String localFileName, String environment) throws IOException {
		StorageReference fileRef = storageRef.child(storageFilePath);

		File root = context.getExternalFilesDir(environment);
		final File localFile = new File(root, localFileName);

		fileRef.getFile(localFile).addOnSuccessListener(new OnSuccessListener<FileDownloadTask.TaskSnapshot>() {
			@Override
			public void onSuccess(FileDownloadTask.TaskSnapshot taskSnapshot) {
				OnDownloadToLocalFileComplete(callback, localFile.getAbsolutePath());
			}
		}).addOnFailureListener(new OnFailureListener() {
			@Override
			public void onFailure(Exception e) {
				OnDownloadToLocalFileError(callback, e.toString());
			}
		}).addOnProgressListener(new OnProgressListener<FileDownloadTask.TaskSnapshot>() {
			@Override
			public void onProgress(@NonNull FileDownloadTask.TaskSnapshot snapshot) {
				float progress = (100.0f * snapshot.getBytesTransferred()) / snapshot.getTotalByteCount();
				OnDownloadToLocalFileProgress(callback, (int) progress);
			}
		});
	}

	public static void getFileMetadata(final long callback, String storageFilePath) {
		StorageReference fileRef = storageRef.child(storageFilePath);

		fileRef.getMetadata().addOnSuccessListener(new OnSuccessListener<StorageMetadata>() {
			@Override
			public void onSuccess(StorageMetadata storageMetadata) {
				OnGetMetadataComplete(callback, storageMetadata);
			}
		}).addOnFailureListener(new OnFailureListener() {
			@Override
			public void onFailure(Exception e) {
				OnGetMetadataError(callback, e.toString());
			}
		});
	}

	public static void updateMetadata(final long callback, String storageFilePath, StorageMetadata.Builder builder) {
		StorageReference fileRef = storageRef.child(storageFilePath);

		StorageMetadata metadata = builder.build();
		fileRef.updateMetadata(metadata).addOnSuccessListener(new OnSuccessListener<StorageMetadata>() {
			@Override
			public void onSuccess(StorageMetadata storageMetadata) {
				OnUpdateMetadataComplete(callback, storageMetadata);
			}
		}).addOnFailureListener(new OnFailureListener() {
			@Override
				public void onFailure(Exception e) {
				OnUpdateMetadataError(callback, e.toString());
			}
		});
	}

	public static void deleteFile(final long callback, final String storageFilePath) {
		StorageReference fileRef = storageRef.child(storageFilePath);

		fileRef.delete().addOnSuccessListener(new OnSuccessListener<Void>() {
			@Override
			public void onSuccess(Void aVoid) {
				OnDeleteFileComplete(callback);
			}
		}).addOnFailureListener(new OnFailureListener() {
			@Override
			public void onFailure(Exception e) {
				OnDeleteFileError(callback, e.toString());
			}
		});
	}

	public static HashMap<String, String> getCustomMetadata(StorageMetadata metadata) {
		HashMap<String, String> result = new HashMap<>();

		for (String key: metadata.getCustomMetadataKeys()) {
			result.put(key, metadata.getCustomMetadata(key));
		}

		return result;
	}

	public static StorageMetadata.Builder getMetadataBuilder() {
		return new StorageMetadata.Builder();
	}
}