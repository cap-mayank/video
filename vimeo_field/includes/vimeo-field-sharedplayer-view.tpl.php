<?php

/**
 * @file
 * Tpl to display set of Ooyala videosyer_id : Unique identifier of the player.
 *
 * - $width : The width of the player.
 * - $height : The height of the player.
 * - $first_embed_code : The first embed code to view.
 * - $embed_codes : An array of embed codes that are included in the player.
 */
?>
  
<div class="ooyala_shared_player">
  <div id="<?php print $video_id; ?>-wrapper">
    <object width="<?php print $width; ?>" height="<?php print $height; ?>">
      <param name="allowfullscre" value="TRUE" />
      <param name="allowscriptaccess" value="always" />
      <param name="movie" value="http://vimeo.com/moogaloop.swf?clip_id=<?php print $video_id[0]; ?>  &server=vimeo.com&color=00adef&fullscreen=1" />
      <embed src="http://vimeo.com/moogaloop.swf?clip_id=<?php print $video_id[0] ?>&server=vimeo.com&color=00adef&fullscreen=1" type="application/x-shockwave-flash" allowfullscreen="true" allowscriptaccess="always" width="<?php print $width; ?>" height="<?php print $height; ?>"> </embed>
    </object>
  </div>
</div>
