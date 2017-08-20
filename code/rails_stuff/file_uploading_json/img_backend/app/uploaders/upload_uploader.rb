# encoding: utf-8

class UploadUploader < CarrierWave::Uploader::Base
  # include CarrierWave::RMagick
  # include CarrierWave::MiniMagick

  storage :file
  def store_dir
    "uploads/#{model.class.to_s.underscore}/#{mounted_as}/#{model.id}"
  end
end
